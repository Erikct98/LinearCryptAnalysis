"""
A file with some random scribbles that might be interesting
"""

from collections import Counter
from distutils import core
import enum
import itertools
import math
import random
from re import L
import sys
from tkinter.filedialog import test
from typing import Dict, Tuple
from core.mixcolumn_trail_search import find_input_mask, find_output_mask
from core.subbytes import mulp_inv
from core.correlation import corr, func_corr, icorr, ipm16, ipms_with_abs_corr, keyed_func_corr, ocorr, opm16, opms_with_abs_corr
from core.toolbox import BITS, I, P8, GF2_8, GF2_8_min_0, bin_, export_table, hex_, join32, signum, split32, subbyte, _subbytes, xor_closure
from core.mmm import MM5_input_masks, MM5_output_masks, core_input_masks, core_output_masks, majority_func_for_masks, weighted_majority_func_for_masks
from core.key_classes import MM3_class_reps, MM3_key_class_corr, MM3_key_rep, MM3_zero_key_class, MM5_class_reps, MM5_key_class_corr, _key_class_corr, orthogonal_class, _class_reps

def vector_corrs_random_perm(opm: int):
    # Input masks
    w1, w2, w3 = next(core_input_masks(opm))

    # Create random permutation
    random_perm = [x for x in GF2_8]
    random.shuffle(random_perm)

    # Compute likelihoods
    vectors = []
    for pt in GF2_8:
        # vec
        pass


def input_mask_combos():
    tuples = [(pt, subbyte(pt)) for pt in GF2_8]
    opm = 0x01
    for m1, m2, m3 in itertools.combinations(GF2_8_min_0, 3):
        # Compute res
        res = {k:0 for k in itertools.product(BITS, repeat=3)}
        for pt, ct in tuples:
            vector = P8(pt & m1), P8(pt & m2), P8(pt & m3)
            res[vector] += I(P8(ct & opm))

        # Check if better
        cnt = Counter(abs(x) for x in res.values())
        if cnt.get(20, 0) > 1:
            print(f"for {hex_(opm)}  with ipms {hex_(m1)}, {hex_(m2)}, {hex_(m3)} we have {cnt(20)} 20s")


def forward_vectors():
    print(hex_(find_input_mask(0x01000000)))
    pairs = [(0xA3, 0x01),  (0xA3, 0x01), (0x9F, 0x81), (0x81, 0x80)]
    for ipm, opm in pairs:
        print(ipm, opm, corr(ipm, opm))

    # input_mask_combos()
    ipm = 0x01
    v1, v2, v3 = next(core_output_masks(ipm))
    opms = [v1, v2, v3]
    for opm in opms:
        print(corr(ipm, opm))
    print([hex_(x) for x in opms])

    # keys
    zkc = orthogonal_class(opms)
    keys = _class_reps(zkc)
    keys = sorted(keys, key=lambda x: tuple(P8(opm & x) for opm in opms))

    cnts = {}
    for k in keys:
        tuples = [(pt, subbyte(pt)^k) for pt in GF2_8]
        vectors = []
        for pt, ct in tuples:
            ipvec = P8(pt & ipm)
            opvec = (P8(opm & ct) for opm in opms)
            vec = (ipvec, *opvec)
            vectors.append(vec)
        cnt = Counter(vectors)
        cnt = {k: cnt[k] for k in itertools.product(BITS, repeat=4)}

        cnts[k] = cnt

    for vec in cnts[keys[0]]:
        print(vec, *[cnts[k][vec] for k in keys])


if __name__ == "__main__":
    forward_vectors()

    sys.exit()


    imm = 0x01000000
    imms = split32(imm)
    rev = find_input_mask(imm)
    ipms = [tuple(hex_(x) for x in next(core_input_masks(imm))) for imm in split32(rev)]

    print(ipms)
    opms = next(core_output_masks(imms[0]))
    print(tuple(hex_(x) for x in opms))
    

    sys.exit()


    opm = 0x2D
    # for opm in GF2_8_min_0:
    print("==============")
    w1, w2, w3 = next(core_input_masks(opm))

    res = []
    for pt, ct in tuples:
        res.append((P8(pt & w2), P8(pt & w3), P8(ct & opm)))
    res = dict(Counter(res))
    res = {k: v for k, v in sorted(res.items(), key=lambda x: (x[0][2], x[0][0], x[0][1]))}
    for k, v in res.items():
        print(k, v)

    # opm = 0x01000000
    # parts = split32(find_input_mask(opm))
    # print("IPMS")
    # for part in parts:
    #     print([hex_(x) for x in next(core_input_masks(part))])

    # print("OPMS")
    # for part in split32(opm):
    #     print([hex_(x) for x in next(core_output_masks(part))])
    # pass
