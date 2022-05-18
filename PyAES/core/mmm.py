"""
Library used to compute information surrounding the
Multiple Mask Method.
"""

from core.correlation import func_corr, ipms_with_abs_corr, opms_with_abs_corr
from core.toolbox import P8, GF2_8_min_0, xorsum
from itertools import combinations, product
from typing import Callable, Iterable, List, Tuple


def MM5_input_masks(opm: int) -> List[int]:
    """
    Returns 5 input masks that give a correlation of +/- 2^-2 with opm `opm`
    when exploited by the 5 Mask Method.
    """
    assert 0 < opm < 256
    return ipms_with_abs_corr(opm, 16)


def MM5_output_masks(ipm: int) -> List[int]:
    """
    Returns 5 output masks that give a correlation of +/- 2^-2 with ipm `ipm`
    when exploited by the 5 Mask Method.
    """
    assert 0 < ipm < 256
    return opms_with_abs_corr(ipm, 16)


def core_input_masks(opm: int) -> Iterable[Tuple[int, int, int]]:
    """
    Yields two trio's of input masks that can be used to create
    the 5 input masks for this `opm`.
    :returns: a tuple with the "leader" in first position.
    """
    assert 0 < opm < 256

    # Find the mask-triples that are linearly dependent.
    sets: List[set] = []
    m5 = MM5_input_masks(opm)
    for m1, m2, m3 in combinations(m5, 3):
        if m1 ^ m2 ^ m3 == 0:
            sets.append(set([m1, m2, m3]))

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


def core_input_masks_v2(opm: int) -> Iterable[Tuple[int, int, int]]:
    """
    Yields all trio's of input masks that can be used to create
    the 5 input masks for this `opm`.
    """
    assert 0 < opm < 256

    m5 = MM5_input_masks(opm)
    for masks in combinations(m5, 3):
        if xorsum(masks) != 0:
            yield tuple(sorted(masks))


def core_output_masks(ipm: int) -> Iterable[Tuple[int, int, int]]:
    """
    Yields two trio's of output masks that can be used to create
    the 5 output masks for this `ipm`.
    :returns: a tuple with the "leader" in first position.
    """
    assert 0 < ipm < 256

    # Find the mask-triples that are linearly dependent.
    sets: List[set] = []
    m5 = MM5_output_masks(ipm)
    for m1, m2, m3 in combinations(m5, 3):
        if m1 ^ m2 ^ m3 == 0:
            sets.append(set([m1, m2, m3]))

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


def majority_func_for_masks(masks: List[int]) -> Callable[[int], int]:
    def f(t: int) -> int:
        return sum(P8(t & m) for m in masks) >= (len(masks) + 1) / 2
    return f


def input_func_for_MM5(opm: int) -> Callable[[int], int]:
    """
    Returns guess function based on opm `opm`.
    with 5 Mask Method.
    """
    return majority_func_for_masks(MM5_input_masks(opm))


def input_func_for_MM3(opm: int) -> Callable[[int], int]:
    """
    Returns guess function based on opm `opm`.
    with 3 Mask Method.
    """
    return majority_func_for_masks(next(core_input_masks(opm)))


def test_MM5_output():
    for opm in GF2_8_min_0:
        ipf = input_func_for_MM5(opm)

        def opf(ct):
            return P8(ct & opm)
        corr128 = func_corr(ipf, opf)
        assert abs(corr128) == 32


def test_MM3_output():
    for opm in GF2_8_min_0:
        ipf = input_func_for_MM3(opm)

        def opf(ct):
            return P8(ct & opm)
        corr128 = func_corr(ipf, opf)
        assert abs(corr128) == 28


if __name__ == "__main__":
    pass
