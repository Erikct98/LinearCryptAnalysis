"""
Library that assists in computing which key guesses give the best
correlation
"""

import numpy as np
from typing import List
from correlation import corr, func_corr, keyed_func_corr, ocorr
from toolbox import GF2_8, P8, signum, subbyte
from SetCoverPy import setcover

def get_32_mapping(opm: int, key: int) -> List[bool]:
    """
    Compute which key guesses give correlation 32/128 when the actual key is
    `key` and we are looking at output mask `opm`.
    :returns: list of booleans, with list[kg] specifying whether key guess
    `kg` gives correlation magnitude 32/128.
    """
    coeffs = {i: signum(c) for i, c in enumerate(ocorr(opm)) if abs(c) >= 16}
    offset = 3 - 5 *P8(opm & subbyte(0))

    # Compute which key guesses give correlation with magnitude 32/128
    res = []
    for kg in GF2_8:
        def ipf(pt):
            total = sum((coeff * P8(ipm & (pt ^ kg))) for ipm, coeff in coeffs.items())
            return total >= offset

        corr128 = keyed_func_corr(ipf, lambda x: P8(x & opm), ipk = key, opk=0)
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


def test_correlation():
    """
    Test that for each key, there is a guess among the key guesses that
    attains correlation 32/128.
    """
    opm = 0x77
    guesses = get_key_guesses(opm)

    coeffs = {i: signum(c) for i, c in enumerate(ocorr(opm)) if abs(c) >= 16}
    offset = 3 - 5 * P8(opm & subbyte(0))

    def opf(ct: int) -> int:
        return P8(ct & opm)

    for key in GF2_8:
        mapping = {}
        for kg in guesses:
            def ipf(pt: int) -> int:
                return sum(coeff * P8(ipm & (pt ^ kg)) for ipm, coeff in coeffs.items()) >= offset

            mapping[kg] = keyed_func_corr(ipf, opf, ipk=key, opk=0)
        assert 32 in mapping.values()


if __name__ == "__main__":
    pass