"""
Library used to investigate the alignment of key classes.
"""

import json
import itertools
from core.mixcolumn_trail_search import find_input_mask
from core.mmm import core_input_masks_v2
from core.toolbox import GF2_8_min_0, split32
from core.key_classes import orthogonal_class, MM5_zero_key_class
from typing import Iterable, List, Tuple, Dict

def intersect_sets(*sets: Iterable) -> set:
    """
    Returns the elements that are present in all sets in `sets`.
    """
    inter = set(sets[0])
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

    return itertools.product(*triple_groups)


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
        opm: [x for x in core_input_masks_v2(opm)]
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
        k: v for k, v in sorted(to_class.items())
    }

    from_class = {
        class_: [k for k, v in to_class.items() if v == class_]
        for class_ in sorted(set(to_class.values()))
    }

    results = {
        "to_class": to_class,
        "from_class": from_class
    }

    with open('sets_v3.json', 'w') as fp:
        json.dump(results, fp)


def align_masks():
    """
    Explore the output mask after 2 rounds and see for which we can create
    a key-aligned trail.
    """

    # Load lookup table
    with open('sets_v3.json', 'r') as fp:
        LUT = json.load(fp)
    to_class = LUT["to_class"]

    for opm in GF2_8_min_0:
        for ipms in core_input_masks_v2(opm):
            iipms = [find_input_mask(ipm << 24)
                     for i, ipm in enumerate(ipms)]
            for idx, masks in enumerate(zip(*[split32(ipm) for ipm in iipms])):
                intersection = intersect_sets(*[MM5_zero_key_class(ipm) for ipm in masks])
                if len(intersection) > 2:
                    print(opm, ipms, [f"0x{ipm:08X}" for ipm in iipms])
                    print("   ", intersection)
            #     sort_m = sorted(masks)
            #     key = str(tuple(sort_m))
            #     if key in keys:
            #         print(f"HIT 0x{opm:02X}", [f"0x{ipm:02X}" for ipm in ipms], [f"0x{ipm:08X}" for ipm in iipms], key, to_class[key])


if __name__ == "__main__":
    pass