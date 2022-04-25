"""
Library used to work with key classes.
"""
from core.mmm import MM5_input_masks
from core.correlation import keyed_func_corr
from core.toolbox import GF2_8, P8, export_table
from typing import List


def orthogonal_class(words: List[int]) -> List[int]:
    """
    Returns all words in `GF2_8` that have 0-parity with all words in `words`.
    """
    return [k for k in GF2_8 if all(P8(k & w) == 0 for w in words)]


def MM5_zero_key_class(opm: int) -> List[int]:
    """
    Returns the 32 integers in the 5MM zero-key-class for `opm`.
    """
    assert 0 < opm < 256
    return orthogonal_class(MM5_input_masks(opm))


def MM5_class_reps(opm: int) -> List[int]:
    """
    Returns list of MM5 key representatives, one for each class.
    """
    assert 0 < opm < 256

    reps = []
    found = []
    zkc = MM5_zero_key_class(opm)
    for i in GF2_8:
        if i ^ zkc[0] not in found:
            reps.append(i)
            found.extend(i ^ z for z in zkc)
    assert len(reps) == 8
    return reps


def MM5_key_rep(opm: int, key: int):
    """
    Returns key representative for this key.
    """
    assert 0 < opm < 256
    assert 0 <= key < 256

    zkc = MM5_zero_key_class(opm)
    reps = MM5_class_reps(opm)
    for rep in reps:
        if key ^ rep in zkc:
            return rep
    raise ValueError("woops")


def key_class_corr(opm):
    "Determine correlation of key class A when key is in class B"

    # Determine the key classes
    reps = MM5_class_reps(opm)
    masks = MM5_input_masks(opm)

    def opf(ct):
        return P8(ct & opm)

    res = []
    for k in reps:
        res.append([])
        for kg in reps:
            def ipf(pt):
                return sum(P8((pt ^ kg) & m) for m in masks) >= 3
            corr128 = keyed_func_corr(ipf, opf, ipk=k, opk=0)
            res[-1].append(abs(corr128))
    export_table(res)


if __name__ == "__main__":
    pass
