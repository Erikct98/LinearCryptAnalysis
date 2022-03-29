"""
Library used to compute information surrounding the
Multiple Mask Method.
"""

from itertools import combinations, product
import json
from typing import Callable, Dict, List, Tuple
from correlation import ocorr
from toolbox import GF2_8, P8, GF2_8_min_0, int_to_hex, xor_closure


def input_func_for_masks(masks: List[int]) -> Callable[[int], int]:
    def opf(pt: int) -> int:
        return sum(P8(pt & ipm) for ipm in masks) >= (len(masks) + 1) / 2
    return opf


def input_func_for_MM5(opm: int) -> Callable[[int], int]:
    """
    Returns guess function based on opm `opm`.
    with 5 Mask Method.
    """
    ipms = MM5_input_masks(opm)
    return input_func_for_masks(ipms)


def MM5_input_masks(opm: int) -> List[int]:
    """
    Returns 5 input masks that give a correlation of 2^-2
    when exploited by the 5 Mask Method.
    """
    assert 0 < opm < 256
    return [i for i, v in enumerate(ocorr(opm)) if abs(v) == 16]


def core_input_masks(opm: int) -> Tuple[int, int, int]:
    """
    Yields all trio's of input masks that can be used to create
    the 5 input masks for this `opm`.
    :returns: a tuple with the "leader" in first position.
    """
    assert 0 < opm < 256

    m5 = MM5_input_masks(opm)

    # Find the mask-triples that are linearly dependent.
    sets: List[set] = []
    for m1, m2, m3 in combinations(m5, 3):
        if m1 ^ m2 ^ m3 == 0:
            sets.append(set((m1, m2, m3)))

    # Should be exactly two triples
    assert len(sets) == 2
    set1, set2 = sets

    # Find element that is in both triples
    intersection = set1.intersection(set2)
    assert len(intersection) == 1
    leader = intersection.pop()
    set1.remove(leader)
    set2.remove(leader)

    for elt1, elt2 in product(set1, set2):
        yield (leader, elt1, elt2)


def zero_key_class(opm: int) -> List[int]:
    """
    Returns the 32 integers in the zero-key-class for `opm`.
    """
    ipms = MM5_input_masks(opm)
    keys = []
    for key in GF2_8:
        if all(P8(key & ipm) == 0 for ipm in ipms):
            keys.append(key)
    return keys


def test_zero_key_class():
    with open('key_guess_mapping_v4.json', 'r') as fp:
        data: Dict = json.load(fp)
    for opm, classes in data.items():
        opm = int(opm, 16)
        assert set(zero_key_class(opm)) == xor_closure(classes[0]), (zero_key_class(opm), xor_closure(classes[0]))


if __name__ == "__main__":
    pass
