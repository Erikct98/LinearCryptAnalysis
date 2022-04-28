"""
Library used to work with key classes.
"""
from core.mmm import MM5_input_masks, core_input_masks
from core.correlation import keyed_func_corr
from core.toolbox import GF2_8, P8, Table, bin_, export_table
from typing import List


def orthogonal_class(words: List[int]) -> List[int]:
    """
    Returns all words in `GF2_8` that have 0-parity with all words in `words`.
    """
    return [k for k in GF2_8 if all(P8(k & w) == 0 for w in words)]


def MM3_zero_key_class(opm: int) -> List[int]:
    """
    Returns the 32 integers in the 3MM zero-key-class for `opm`.
    """
    assert 0 < opm < 256
    return orthogonal_class(next(core_input_masks(opm)))


def MM5_zero_key_class(opm: int) -> List[int]:
    """
    Returns the 32 integers in the 5MM zero-key-class for `opm`.
    """
    assert 0 < opm < 256
    return orthogonal_class(MM5_input_masks(opm))


def _class_reps(zkc: List[int]) -> List[int]:
    """
    Compute class representatives for `opm` and `zkc`.
    :param zkc: zero key class.
    """
    reps = []
    found = []
    for i in GF2_8:
        if i ^ zkc[0] not in found:
            reps.append(i)
            found.extend(i ^ z for z in zkc)
    return reps


def MM3_class_reps(opm: int) -> List[int]:
    """
    Returns list of MM3 key representatives, one for each class.
    """
    assert 0 < opm < 256
    zkc = MM3_zero_key_class(opm)
    reps = _class_reps(zkc)
    assert len(reps) == 8
    return reps


def MM5_class_reps(opm: int) -> List[int]:
    """
    Returns list of MM5 key representatives, one for each class.
    """
    assert 0 < opm < 256
    zkc = MM5_zero_key_class(opm)
    reps = _class_reps(zkc)
    assert len(reps) == 8
    return reps


def _key_rep(key: int, reps: List[int], zkc: List[int]) -> int:
    """
    Returns key representative for this key.
    """
    for rep in reps:
        if key ^ rep in zkc:
            return rep
    raise ValueError("woops")


def MM3_key_rep(opm: int, key: int):
    """
    Returns key representative for this key for MM3.
    """
    assert 0 < opm < 256
    assert 0 <= key < 256
    zkc = MM3_zero_key_class(opm)
    reps = MM3_class_reps(opm)
    return _key_rep(key, reps, zkc)


def MM5_key_rep(opm: int, key: int):
    """
    Returns key representative for this key for MM5.
    """
    assert 0 < opm < 256
    assert 0 <= key < 256
    zkc = MM5_zero_key_class(opm)
    reps = MM5_class_reps(opm)
    return _key_rep(key, reps, zkc)


def _key_class_corr(opm, reps, masks, th) -> Table:
    """
    Determine correlation when guessing key class A and key is in class B
    :param opm: output mask
    :param reps: key class representatives
    :param masks: masks used in majority function
    :returns: table with results.
    """
    def opf(ct):
        return P8(ct & opm)

    table = []
    for k in reps:
        table.append([])
        for kg in reps:
            def ipf(pt):
                return sum(P8((pt ^ kg) & m) for m in masks) >= th
            corr128 = keyed_func_corr(ipf, opf, ipk=k, opk=0)
            table[-1].append(corr128)
    export_table(table)


def MM3_key_class_corr(opm):
    """
    Determine correlation when guessing key class A and key is in class B
    and utilizing the 3MM.
    :param opm: output mask
    """
    # Determine the key classes
    reps = MM3_class_reps(opm)
    masks = next(core_input_masks(opm))

    print([bin_(m) for m in masks])
    _key_class_corr(opm, reps, masks, 2)


def MM5_key_class_corr(opm):
    """
    Determine correlation when guessing key class A and key is in class B
    and utilizing the 5MM.
    :param opm: output mask
    """
    # Determine the key classes
    reps = MM5_class_reps(opm)
    masks = MM5_input_masks(opm)

    _key_class_corr(opm, reps, masks, 3)


if __name__ == "__main__":
    pass
