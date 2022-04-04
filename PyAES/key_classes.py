"""
Library used to investigate the alignment of key classes.
"""

import json
from mmm import core_input_masks
from toolbox import GF2_8, GF2_8_min_0, P8, xor_closure
from typing import Counter, Dict, List


def orthogonal_class(masks: List[int]) -> List[int]:
    """
    Returns all masks in `GF2_8` that have 0-parity with all masks in `masks`.
    """
    return [k for k in GF2_8 if all(P8(k & m) == 0 for m in masks)]


def zero_key_class(opm: int) -> List[int]:
    """
    Returns the 32 integers in the 5MM zero-key-class for `opm`.
    """
    return orthogonal_class(next(core_input_masks(opm)))


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

    def find_intersection(triples1, triples2, triples3):
        for t1 in triples1:
            for t2 in triples2:
                for t3 in triples3:
                    intersection = set(t1).intersection(t2).intersection(t3)
                    if intersection:
                        return intersection
        return None

    results = {}
    for opm1 in GF2_8_min_0:
        for opm2 in range(opm1 + 1, 256):
            for opm3 in range(opm2 + 1, 256):
                intersection = find_intersection(triples[opm1], triples[opm2], triples[opm3])
                if intersection:
                    print(opm1, opm2, opm3, intersection, triples[opm1], triples[opm2], triples[opm3])

    #                 zkc1 = zero_key_class(opm1)
    #                 zkc2 = zero_key_class(opm2)
    #                 zkc3 = zero_key_class(opm3)
    #                 czks = set(zkc1).intersection(zkc2).intersection(zkc3)

    #                 key = str(sorted(czks))
    #                 results.setdefault(opm1, {})
    #                 results[opm1].setdefault(key, [])
    #                 results[opm1][key].append((opm1, opm2, opm3))
    #                 results.setdefault(opm2, {})
    #                 results[opm2].setdefault(key, [])
    #                 results[opm2][key].append((opm1, opm2, opm3))
    #                 results.setdefault(opm3, {})
    #                 results[opm3].setdefault(key, [])
    #                 results[opm3][key].append((opm1, opm2, opm3))

    # results = {k: results[k] for k in sorted(results)}

    # with open('sets.json', 'w') as fp:
    #     json.dump(results, fp)


def key_class_alignment_pt2():
    """
    Some experiments
    """
    with open('sets.json', 'r') as fp:
        data: Dict[str, Dict[str, List[List[int]]]] = json.load(fp)

    mask_classes = []
    for opm, classes in data.items():
        opm = int(opm)
        assert len(classes) == 1
        class_ = list(classes.keys())[0]

        # Find which members are also part of this class
        members = []
        for m, classes in data.items():
            if class_ in classes:
                members.append(int(m))

        # See if this class is already known.
        if members not in mask_classes:
            mask_classes.append(members)

    # Investigate how often each neighbour is used
    for opm, classes in data.items():
        opm = int(opm)
        assert len(classes) == 1
        class_ = list(classes.keys())[0]
        triples = classes[class_]

        concat = [x for y in triples for x in y]
        print(opm, Counter(concat))

    # Investigate number of tuples per class
    for class_ in mask_classes:
        all_triples = []
        for elt in class_:
            triples = list(data[str(elt)].values())[0]
            for triple in triples:
                if triple not in all_triples:
                    all_triples.append(triple)
        print(class_, all_triples, len(all_triples))


def test_zero_key_class():
    with open('key_guess_mapping_v4.json', 'r') as fp:
        data: Dict = json.load(fp)
    for opm, classes in data.items():
        opm = int(opm, 16)
        assert set(zero_key_class(opm)) == xor_closure(classes[0]), (zero_key_class(opm), xor_closure(classes[0]))


if __name__ == "__main__":
    pass
    key_class_alignment()