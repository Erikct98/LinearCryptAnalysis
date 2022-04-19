"""
A file with some random scribbles that might be interesting
"""

from collections import Counter
from itertools import chain, combinations, product
import math
from random import randint, random
from re import L
import sys
from tkinter import E
from key_classes import orthogonal_class, zero_key_class
from mmm import MM5_input_masks, core_input_masks, core_input_masks_v2, input_func_for_MM5, majority_func_for_masks
from mixcolumn_trail_search import find_input_mask, find_output_mask
from key_guessing import get_key_guesses
from correlation import corr, func_corr, icorr, ipm16, keyed_func_corr, ocorr, opm16
from mixcolumn import mixcolumn
from toolbox import GF2_8, I, J, P32, P8, GF2_8_min_0, bit_count, export_table, inv_subbyte, orsum, rand32, signum, subbyte, transpose, _subbytes, join32, split32, xor_closure, xorsum
from subbytes import compute_subbytes_LAT, inv_transform
from LAT import LAT
from KAT import KAT
from SetCoverPy import setcover
import numpy as np

def MM2():
    opm = 0x01
    masks = MM5_input_masks(opm)
    map = {
        pt: subbyte(pt)
        for pt in GF2_8
    }

    nr_masks = 2

    # res = {}
    # for ms in combinations(masks, nr_masks):
    #     cnt = {}
    #     for pt, ct in map.items():
    #         key = (*[P8(pt & m) for m in ms], P8(ct & opm))
    #         cnt.setdefault(key, 0)
    #         cnt[key] += 1
    #     cnt = {
    #         k: v for k, v in sorted(cnt.items())
    #     }
    #     res[ms] = cnt
    # print(res)

    # # Investigate results
    # for masks, cnts in res.items():
    #     max_cnt = 0
    #     for bits in product(*([range(2)] * nr_masks)):
    #         if (*bits, 0) in cnts and (*bits, 1) in cnts:
    #             max_cnt += max(cnts[(*bits, 0)], cnts[(*bits, 1)])
    #     print(masks, max_cnt)


    def opf(ct):
        return P8(ct & opm)

    res = {}
    best = 0
    info = None
    for ms in combinations(GF2_8_min_0, nr_masks):
        def ipf(pt):
            return orsum(P8(pt & m) for m in ms)

        corr128 = func_corr(ipf, opf)
        # print(ms, corr128)
        if abs(corr128) > best:
            best = abs(corr128)
            info = best, ms

    print(best, info)


def MM4():
    opm = 0x01
    ipms = [0xA3, 0x2D, 0xC4, 0x4A]

    def opf(ct):
        return P8(ct & opm)

    for i in range(5):
        def ipf(pt):
            return sum(P8(pt & m) for m in ipms) >= i

        corr128 = func_corr(ipf, opf)
        print(i, corr128)



def parity_trail():
    opm = 0x01

    def opf(ct):
        return P8(ct & opm)

    best, info = 0, None
    for ipms in combinations(range(1, 256), 2):
        def ipf(pt):
            return sum(P8(pt & ipm) for ipm in ipms) & 0x01
        corr128 = func_corr(ipf, opf)
        if abs(corr128) > abs(best):
            best = corr128
            info = ipms
        print(ipms, corr128)
    print(best, info)

if __name__ == "__main__":
    # parity_trail()
    # MM4()

    # opm = 0x01
    # ipmss = [f for f in core_input_masks_v2(opm)]
    # for ipms in ipmss:
    #     masks = xor_closure(ipms)
    #     masks.remove(0)
    #     print(masks, len(masks))
    #     ipf = majority_func_for_masks(masks)
    #     def opf(ct):
    #         return P8(ct & opm)
    #     for key in GF2_8:
    #         print(keyed_func_corr(ipf, opf, ipk=key, opk=0))

    # opm = 0x01
    # ipms = [0xA3, 0x2D, 0xC4]

    # ipk = 0x9A
    # opk = 0x53
    # res = {}
    # for pt in GF2_8:
    #     ct = opk ^ subbyte(ipk ^ pt)

    #     oth = (P8(ipms[0] & (pt ^ ipk)) & P8((ipms[1] ^ ipms[2]) & (pt ^ ipk))) ^ ((P8(ipms[1] & (pt ^ ipk)) & P8(ipms[2] & (pt ^ ipk))))
    #     maj = int(sum(P8((pt ^ ipk) & ipm) for ipm in ipms) >= 2)
    #     # majk = sum(P8(ipk & ipm) for ipm in ipms) >= 2

    #     lp = maj
    #     rp = oth

    #     idx = f"{lp}{rp}"
    #     res.setdefault(idx, 0)
    #     res[idx] += 1
    # print(res)

    # import random

    # grid = [[random.randint(0, 0xFF) for _ in range(4)] for _ in range(4)]

    # ggrid = [[subbyte(x) for x in y] for y in grid]

    # for g in grid:
    #     print(" & ".join(["\hex{" + f"{r:02X}" + "}" for r in g]))
    # print()
    # for g in ggrid:
    #     print(" & ".join(["\hex{" + f"{r:02X}" + "}" for r in g]))

    key = 0x7bb63927
    key_guess = 0x48e9b8b0
    random_key = 0xd05df7a6
    inv = 0x335f8197

    oopm = 0x2D000000
    opm = find_input_mask(oopm)
    opms = split32(opm)
    for i, v in enumerate(zip(opms, split32(key), split32(key_guess), split32(random_key), split32(inv))):
        opm, k, kg, rkg, i = v
        ipms = next(core_input_masks(opm))
        zkc = orthogonal_class(ipms)
        for ipm in ipms:
            print(f'opm: {hex(opm)}, ipm: {hex(ipm)}, k: {hex(k)}, kg: {hex(kg)}, rkg: {hex(rkg)}, i: {hex(i)}')
            kp = [P8(ipm & (k ^ z)) for z in GF2_8]
            kgp = [P8(ipm & (kg ^ z)) for z in GF2_8]
            rkgp = [P8(ipm & (rkg ^ z)) for z in GF2_8]
            print("kp, kgp ", sum(i == j for i, j in zip(kp, kgp)))
            print("kp, rkgp", sum(i == j for i, j in zip(kp, rkgp)))
            print("kp, i   ", sum(i == j for i, j in zip(kp, ip)))



    sys.exit()

    masks = [
        0x5209DC81,
        0xABFADFC9,
        0x9165091E,
        0x25B83709,
    ]

    reverse = transpose([list(split32(m)) for m in masks])
    print([[hex(x) for x in r] for r in reverse])
    reverse = [join32(*x) for x in reverse]
    print([hex(r) for r in reverse])

    mixed = [find_input_mask(x) for x in reverse]
    print([hex(r) for r in mixed])
    reverse = transpose([list(split32(m)) for m in mixed])
    print([[hex(x) for x in r] for r in reverse])
    for g in reverse:
        print(" & ".join(["\hex{" + f"{r:02X}" + "}" for r in g]))

    sys.exit()


    opm = 0x2D
    print(MM5_input_masks(opm))
    m2, m3, m4, m1, m5 = MM5_input_masks(opm)

    print(m1, m2, m3, m1 ^ m2, m1 ^ m3)
    for m in (m1, m2, m3, m4, m5):
        print(f"0x{m:02X}")

    print(corr(0x93, 0x2D))
    print(corr(0x1A, 0x2D))
    print(corr(0x3A, 0x2D))
    print(corr(0x89, 0x2D))
    print(corr(0xA9, 0x2D))

    sys.exit()



    eligible_keys_1 = [key for key in GF2_8 if P8(m1 & key) == 0 and P8(m2 & key) == 1 and P8(m3 & key) == 0]
    eligible_keys_2 = [key for key in GF2_8 if P8(m1 & key) == 1 and P8(m2 & key) == 0 and P8(m3 & key) == 1]

    key1 = eligible_keys_1[0]
    key2 = eligible_keys_2[0]

    ipms = [m1, m2, m3, m1^m2, m1^m3]

    def opf(ct):
        return P8(ct & opm)

    for key1 in eligible_keys_1:
        def ipf(pt):
            return sum(I(P8((pt ^ key1) & ipm)) for ipm in ipms) < 0


        print(keyed_func_corr(ipf, opf, ipk=key2, opk=0))



    




    # # MM2()
    # oopm = 0x01
    # iipms = next(core_input_masks(oopm))
    # for iipm in iipms:
    #     print(f"IIPM: 0b{iipm:08b} = 0x{iipm:02X}:")
    #     opm = find_input_mask(iipm << 24)
    #     opms = split32(opm)
    #     for opm in opms:
    #         print(f"    OPM: 0b{opm:08b} = 0x{opm:02X}: ")
    #         for ipms in core_input_masks(opm):
    #             print("       ", [f"0b{ipm:08b} = 0x{ipm:02X}" for ipm in ipms])