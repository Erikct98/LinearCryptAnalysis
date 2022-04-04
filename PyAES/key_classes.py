"""
Library used to investigate the alignment of key classes.
"""

from itertools import product
import json
from mixcolumn_trail_search import find_input_mask
from mmm import core_input_masks
from toolbox import GF2_8, GF2_8_min_0, P8, split32, xor_closure
from typing import Dict, Iterable, List, Tuple


def orthogonal_class(words: List[int]) -> List[int]:
    """
    Returns all words in `GF2_8` that have 0-parity with all words in `words`.
    """
    return [k for k in GF2_8 if all(P8(k & w) == 0 for w in words)]


def zero_key_class(opm: int) -> List[int]:
    """
    Returns the 32 integers in the 5MM zero-key-class for `opm`.
    """
    return orthogonal_class(next(core_input_masks(opm)))


def intersect_sets(*sets: set) -> set:
    """
    Returns the elements that are present in all sets in `sets`.
    """
    inter = sets[0].copy()
    for s in sets[1:]:
        inter.intersection_update(s)
    return inter


def intersections(*triple_groups: List[Tuple]) -> Iterable:
    """
    Returns a generator that iterates through all combinations of triples
    (one from each triple group) that have a non-zero intersection.
    """
    # Find which elements are in common across all triple-groups
    sets = [set(e for t in tg for e in t) for tg in triple_groups]
    core = intersect_sets(*sets)

    # Stop if the core is empty
    if not core:
        return []

    # Clean up triple_groups: ignore triples that do not contain the whole core.
    triple_groups = [
        [e for e in tg if core.intersection(e) == core]
        for tg in triple_groups
    ]

    return product(*triple_groups)

def upper_triangular():
    for i in range(1, 256):
        for j in range(i + 1, 256):
            for k in range(j + 1, 256):
                yield (i, j, k)


def key_class_alignment():
    """
    Investigate whether it is possible to set up our linear trail in such a way
    that we can make as few key guesses as possible.
    I.e. attempt to perform key guess alignment.
    """

    # All triples that can be used to attain 2^-2 corr with opm.
    triples = {
        opm: [x for x in core_input_masks(opm)]
        for opm in GF2_8_min_0
    }

    results: Dict[int, Dict[str, set[Tuple[int]]]] = {}
    for opms in upper_triangular():
        for inters in intersections(*[triples[opm] for opm in opms]):
            zkcs = [set(orthogonal_class(i)) for i in inters]
            izkc = intersect_sets(*zkcs)

            key = str(sorted(izkc))
            for opm in opms:
                results.setdefault(opm, {})
                results[opm].setdefault(key, set())
                results[opm][key].add(opms)

    # Check that only one key class is associated with each opm
    for opm, classes in results.items():
        assert len(classes) == 1

    # Compress results
    to_class = {
        str(tupe): class_
        for res in results.values()
        for class_, tupes in res.items()
        for tupe in tupes
    }
    to_class = {
        k: v for k,v in sorted(to_class.items())
    }

    from_class = {
        class_: [k for k, v in to_class.items() if v == class_]
        for class_  in sorted(set(to_class.values()))
    }


    results = {
        "to_class": to_class,
        "from_class": from_class
    }

    with open('sets_v2.json', 'w') as fp:
        json.dump(results, fp)


def align_masks():
    """
    Explore the output mask after 2 rounds and see for which we can create
    a key-aligned trail.
    """

    # Load lookup table
    with open('sets_v2.json', 'r') as fp:
        LUT = json.load(fp)
    to_class = LUT["to_class"]

    for opm in GF2_8_min_0:
        for ipms in core_input_masks(opm):
            ipms = [find_input_mask(ipm << (8 * (3 - i))) for i, ipm in enumerate(ipms)]

            for masks in zip(*[split32(ipm) for ipm in ipms]):
                sort_m = sorted(masks)
                key = str(sort_m)
                if key in to_class:
                    print("HIT", opm, key)



def test_zero_key_class():
    with open('key_guess_mapping_v4.json', 'r') as fp:
        data: Dict = json.load(fp)
    for opm, classes in data.items():
        opm = int(opm, 16)
        assert set(zero_key_class(opm)) == xor_closure(
            classes[0]), (zero_key_class(opm), xor_closure(classes[0]))


if __name__ == "__main__":
    pass

    # key_class_alignment()
    align_masks()
