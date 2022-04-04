"""
A file with some random scribbles that might be interesting
"""

from collections import Counter
from itertools import chain, combinations
import math
from random import randint, random
import sys
from tkinter import E
from mmm import MM5_input_masks, core_input_masks, input_func_for_MM5, majority_func_for_masks
from mixcolumn_trail_search import find_input_mask, find_output_mask, join32mask, split32mask
from key_guessing import get_key_guesses
from correlation import corr, func_corr, icorr, ipm16, keyed_func_corr, ocorr, opm16
from mixcolumn import mixcolumn
from toolbox import GF2_8, I, J, P32, P8, GF2_8_min_0, bit_count, export_table, inv_subbyte, rand32, signum, subbyte, transpose, _subbytes
from subbytes import compute_subbytes_LAT, inv_transform
from LAT import LAT
from KAT import KAT
from SetCoverPy import setcover
import numpy as np


def check_coeff_ipm():
    """
    Double check that these coefficients are correct
    """
    opm = 0x91
    coeffs = [0, 0, -10, 6, 2, -2, -4, -8, 14, -2, 0, 8, 4, 0, 2, 6, -12, -8, -10, -14, -2, -10, -4, 12, -6, 6, 8, 4, 8, 8, 10, -6, 12, 4, 10, 2, -6, -10, -12, 0, -2, 6, -8, 8, -8, 4, -10, -6, -8, 12, 2, -2, -2, -2, 12, -12, 10, 6, 0, -4, 12, 4, -2, -10, 2, 2, 4, 12, -16, -4, -2, 2, 0, -8, -2, -10, 2, 6, -12, 8, 6, -6, -4, 0, -12, -4, -2, -10, -12, 8, -10, 2, 6, -2, -12, 4, 2, 10, 4, 4, 12, -8, 2, -10, 12, -4, -6, -6, 2, 6, 12, -16, 14, 2, -12, 8, -8, 8, 10, -6, 0, 4, 2, 14, 6, 6, 12, -
              12, 4, -8, 14, 2, 6, 6, -12, -12, -6, -10, 0, 4, 0, 8, -14, 2, -12, 12, 2, -14, -10, 2, 0, -12, 2, -6, -4, 4, -8, -12, 6, 2, -4, 0, -2, 2, 2, 10, -16, -8, 6, 2, -12, -8, -8, 8, -6, -14, -4, -4, 2, -6, 2, -2, -4, 0, 6, -10, 8, 8, 8, -12, -10, 2, -10, -6, -12, 0, -12, -12, -2, 6, -4, -16, -10, 10, 14, 14, -4, -4, 6, 14, 0, 8, 12, 8, -6, 6, -4, -4, 2, 10, 6, -6, -8, 4, -14, 6, -16, 12, 4, -4, 6, 6, 4, 8, -2, 2, -14, -6, -8, 0, -14, 2, -4, 12, -4, -8, 2, 14, 12, -12, 2, -14, 2, 6, 12, 8]

    corr128 = 128
    for pt in GF2_8:
        input_parity = J(sum(coeffs[ipm] * I(P8(ipm & pt))
                             for ipm in GF2_8) >> 8)
        output_parity = P8(opm & subbyte(pt))
        corr128 -= input_parity ^ output_parity
    return corr128


def check_corr(ipm, opm):
    corr128 = 128
    for pt in GF2_8:
        in_parity = P8(ipm & pt)
        out_parity = P8(opm & subbyte(pt))
        corr128 -= in_parity ^ out_parity
    return corr128


def check_mixcol(ipm, opm):
    corr1024 = 1024
    for _ in range(0x100):
        pt = rand32()
        in_parity = P32(pt & ipm)
        ct = mixcolumn(pt)
        out_parity = P32(opm & ct)
        corr1024 -= in_parity ^ out_parity
    return corr1024


def remove_terms_experiment():
    """
    Test if this can be done with fewer terms than all 256.
    Result: we seem to be able to have 100% information with only 81 terms with highest correlation.
    Increase to 115 terms and the distinguisher is a simple split.
    """
    opm = 0x91
    coeffs = ocorr(opm)
    offset = P8(opm & subbyte(0))

    normal_results = []
    for pt in GF2_8:
        input_parity = offset + \
            (sum(coeffs[ipm] * P8(ipm & pt) for ipm in GF2_8) >> 7)
        output_parity = P8(opm & subbyte(pt))
        normal_results.append(input_parity ^ output_parity)

    # coeffsv2 = {i: x * c for i, c in enumerate(coeffs) for x in (1, 3) if abs(c) > 10 and (x == 1 or abs(c) in (12, 14))}
    # coeffsv2 = {i: (1 + (abs(c) == 14)) * c for i, c in enumerate(coeffs) if abs(c) >= 6}
    coeffsv2 = {i: c for i, c in enumerate(coeffs) if abs(c) >= 8}
    print(len(coeffsv2.items()))
    offset = 0
    fast_results = []
    for pt in GF2_8:
        input_parity = offset - (sum(coeff * P8(ipm & pt)
                                     for ipm, coeff in coeffsv2.items()) <= -6)
        output_parity = P8(opm & subbyte(pt))
        fast_results.append(input_parity ^ output_parity)

    print("max", max(sum(coeff * P8(ipm & pt) for ipm, coeff in coeffsv2.items())
                     for pt in GF2_8 if sum(coeffs[ipm] * P8(ipm & pt) for ipm in GF2_8) == -128))
    print("min", min(sum(coeff * P8(ipm & pt) for ipm, coeff in coeffsv2.items())
                     for pt in GF2_8 if sum(coeffs[ipm] * P8(ipm & pt) for ipm in GF2_8) == 0))

    x = [(sum(coeff * P8(ipm & pt) for ipm, coeff in coeffsv2.items()),
          sum(coeffs[ipm] * P8(ipm & pt) for ipm in GF2_8)) for pt in GF2_8]
    print(sorted(x))

    print(normal_results, fast_results)
    print(len(coeffsv2.values()))

    corr128 = sum(a ^ b for a, b in zip(normal_results, fast_results))
    return corr128


def corr_test():
    # opm = 0x15
    # coeffs = ocorr(opm)
    # offset = P8(opm & subbyte(0))
    # coeffs = {i: signum(c) for i, c in enumerate(coeffs) if abs(c) >= 16}
    # cut_off = 3 - offset * 5

    # GF2_8 #range(256)[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
    GUESSES = [0, 1, 16, 17, 128, 129, 144, 145]

    # Compute sets
    for opm in range(1, 256):
        coeffs = ocorr(opm)
        offset = P8(opm & subbyte(0))
        coeffs = {i: signum(c) for i, c in enumerate(coeffs) if abs(c) >= 16}
        cut_off = 3 - offset * 5

        hitmask = 0
        for kg in GUESSES:
            def ipf(pt):
                return sum((coeff * P8(ipm & (pt ^ kg))) for ipm, coeff in coeffs.items()) >= cut_off

            res = [keyed_func_corr(ipf, lambda x: P8(
                x & opm), ipk=key, opk=0) for key in GF2_8]
            bitstr = "".join([str(int(abs(c) == 32)) for c in res])
            print(opm, kg, bitstr)
            bits = int(bitstr, 2)
            hitmask |= bits
            print("total",  f"{hitmask:0>255b}")

        print(opm, all(x == 1 for x in res))

        # print(kg, sum(abs(c) for c in res), Counter(res), ''.join([str(int(abs(i) == 32)) for i in res]))

    # matrix = []
    # for guess in GUESSES:
    #     def ipf(pt):
    #         return (sum(coeff * P8(ipm & (pt ^ guess)) for ipm, coeff in coeffs.items()) >= cut_off)

    #     res = [keyed_func_corr(ipf, lambda x: P8(x & opm), ipk = key, opk=0) for key in GF2_8]
    #     print(guess, sum(abs(c) for c in res), Counter(res), ''.join([str(int(abs(i) == 32)) for i in res]))
    #     # matrix.append((guess, [int(abs(i) == 32) for i in res]))

    # mapping = {}
    # for key in GF2_8:
    #     mapping[key] = tuple(guess for guess in range(len(GUESSES)) if matrix[guess][1][key])

    # print(mapping)
    # print(Counter(mapping.values()).most_common(10))

    # matrix.sort(key=lambda x: x[1])
    # for key, res in matrix:
    #     print(f'0x{key:3X}', ''.join([str(i) for i in res]))

    # transpose(matrix)

    # # Create setcover matrix
    # cost = [1] * 256

    # # solve set cover
    # g = setcover.SetCover(np.array(matrix), np.array(cost))
    # print(g.greedy())
    # print(g.SolveSCP())
    # print(g.s)


def build_KAT():
    """
    Build LAT between keys
    """
    sq_lat = [[corr(ipm, opm)**2 for opm in GF2_8] for ipm in GF2_8]

    KAT = [[None for _ in GF2_8] for __ in GF2_8]
    for key_diff in GF2_8:
        for opm in GF2_8:
            KAT[key_diff][opm] = int(
                sum(sq_lat[ipm][opm] * I(P8(ipm & key_diff)) for ipm in GF2_8) / 128)
    print(KAT)
    # export_table(KAT, "KAT.txt")


def stats_on_KAT():
    abskat = [abs(KAT[x][1]) for x in GF2_8]
    abslat = [abs(x) for x in LAT[1]]
    print(sum(x * y for x, y in Counter(abskat).items()) / 256)
    print(sum(x * y for x, y in Counter(abslat).items()) / 256)

    print(list(map(lambda x: f"{x[0]:08b}", filter(
        lambda x: abs(x[1]) == 16, enumerate(abskat)))))

    # row_sums = [sum(abs(KAT[key_diff][x]) for x in GF2_8) for key_diff in GF2_8]
    # print(Counter(row_sums), row_sums)
    # col_sums = [sum(abs(KAT[x][opm]) for x in GF2_8) for opm in GF2_8]
    # print(Counter(col_sums), col_sums)

    # delta_sums = [(sum(abs(KAT[delta ^ opm][opm]) for opm in GF2_8), delta) for delta in GF2_8]
    # print(sorted(delta_sums))


def test_new_correlation():
    GF = range(1, 256)

    corrs = {}
    for opm in GF:
        masks = [i for i, e in enumerate(ocorr(opm)) if abs(e) == 16]

        def ipf(pt):
            Q = sum(I(P8(pt & ipm)) for ipm in masks)
            return J(int(Q/abs(Q)))

        def opf(ct):
            return P8(ct & opm)

        corrs[opm] = func_corr(ipf, opf)
        assert abs(corrs[opm]) == 32
    print(corrs)


def test_masks():
    for opm in GF2_8_min_0:
        masks = MM5_input_masks(opm)
        inv = inv_subbyte(opm)
        print(opm, inv_transform(opm), inv, masks)

def linear_trail_mulp_inv():
    mulp_inv = [inv_transform(x) for x in _subbytes]
    LAT = [
        [128 - sum(P8(pt & ipm ^ mulp_inv[pt] & opm) for pt in GF2_8) for ipm in GF2_8]
        for opm in GF2_8
    ]
    export_table(LAT)

if __name__ == "__main__":
    # test_masks()
    # linear_trail_mulp_inv()

    opm = 0x4A
    ipms = next(core_input_masks(opm))

    def opf(ct):
        return P8(ct & opm)

    res = {}
    for offset1 in GF2_8:
        for offset2 in GF2_8:

            def ipf(pt):
                x = P8(pt & ipms[0])
                x += P8((pt + offset1) & ipms[1])
                x += P8((pt + offset2) & ipms[2])
                return x >= 2

            corr128 = abs(func_corr(ipf, opf))
            if corr128 >= 40:
                res.setdefault(corr128, [])
                res[corr128].append((offset1, offset2))
    print(res)

    pass



    sys.exit()

    mmm = 0x01
    assert 0x2D in MM5_input_masks(0x01)
    opm1 = find_input_mask(0x2D000000)
    opms = split32mask(opm1)
    ipm = join32mask(*[ipm16(opm) for opm in opms])

    print(f"mask befor round 1: 0x{ipm:08X}")
    print(f"mask after sbox round 1: 0x{opm1:08X}")
    print(f"mask after round 1: 0x{0x2D000000:08X}")

    opm = find_output_mask(0x01)
    print(f"mask after round 2: 0x{opm:08X}")

    ipm = opm
    opm = join32mask(*[opm16(ipm) for ipm in split32mask(ipm)])
    print(f"mask after sbox in round 3: 0x{opm:08X}")

    ipms = split32mask(opm)
    for i, ipm in enumerate(ipms):
        print(f"{i}th mask after round 3: 0x{find_output_mask(ipm << (8 * ( 3-i ))):08X}")
    pass