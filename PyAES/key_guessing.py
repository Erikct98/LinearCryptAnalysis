"""
Library that assists in computing which key guesses give the best
correlation
"""

import json
import numpy as np
from typing import List
from correlation import corr, func_corr, keyed_func_corr, ocorr
from toolbox import GF2_8, I, J, P8, signum, subbyte
from SetCoverPy import setcover
from mmm import MM5_input_masks, input_func_for_MM5


def get_32_mapping(opm: int, key: int) -> List[bool]:
    """
    Compute which key guesses give correlation 32/128 when the actual key is
    `key` and we are looking at output mask `opm`.
    :returns: list of booleans, with list[kg] specifying whether key guess
    `kg` gives correlation magnitude 32/128.
    """
    coeffs = {i: signum(c) for i, c in enumerate(ocorr(opm)) if abs(c) >= 16}
    offset = 3 - 5 * P8(opm & subbyte(0))

    # Compute which key guesses give correlation with magnitude 32/128
    res = []
    for kg in GF2_8:
        def ipf(pt):
            total = sum((coeff * P8(ipm & (pt ^ kg)))
                        for ipm, coeff in coeffs.items())
            return total >= offset

        corr128 = keyed_func_corr(ipf, lambda x: P8(x & opm), ipk=key, opk=0)
        res.append(abs(corr128) == 32)
    return res


def get_key_guesses(opm: int) -> List[int]:
    """
    Get set of (8) key guesses that make it possible for `opm` to extract
    3 bits of information on key.
    """

    matrix = [get_32_mapping(opm, key) for key in GF2_8]

    # Solve SetCover problem
    cost = [1] * 256
    g = setcover.SetCover(np.array(matrix), np.array(cost))
    g.SolveSCP()

    return list(map(lambda elt: elt[0], filter(lambda elt: elt[1], enumerate(g.s))))

def get_key_guess(opm: int, key: int) -> List[int]:
    """
    Get key guess that gives 2^-2 correlation for opm `opm` when actual key is `key`.
    """

    with open("key_guess_mapping_v4.json") as fp:
        data = json.load(fp)
    all_guesses = data[f"{opm:02X}"]

    guesses = [x[0] for x in all_guesses]

    def opf(ct):
        return P8(ct & opm)

    masks = MM5_input_masks(opm)
    for guess in guesses:

        def ipf(pt):
            return J(signum(sum(I(P8((pt ^ guess) & ipm)) for ipm in masks)))

        corr128 = keyed_func_corr(ipf, opf, ipk=key, opk=0)
        if abs(corr128) == 32:
            return guess


def test_correlation_for_opm_and_key(opm, key, guesses):
    """
    Test that for `opm`, there is one key among `guesses` that has correlation
    32/128 with `key`.
    """
    coeffs = {i: signum(c) for i, c in enumerate(ocorr(opm)) if abs(c) >= 16}
    offset = 3 - 5 * P8(opm & subbyte(0))

    def opf(ct: int) -> int:
        return P8(ct & opm)

    mapping = {}
    for kg in guesses:
        def ipf(pt: int) -> int:
            return sum(c * P8(ipm & (pt ^ kg)) for ipm, c in coeffs.items()) >= offset

        mapping[kg] = keyed_func_corr(ipf, opf, ipk=key, opk=0)
    assert 32 in mapping.values()
    print(f"{opm:02X}", f"{key:02X}", guesses, mapping)


def test_correlation(opm=0x77):
    """
    Test that for each key, there is a guess among the key guesses that
    attains correlation 32/128.
    """
    guesses = get_key_guesses(opm)
    for key in GF2_8:
        test_correlation_for_opm_and_key(opm, key, guesses)


if __name__ == "__main__":
    pass