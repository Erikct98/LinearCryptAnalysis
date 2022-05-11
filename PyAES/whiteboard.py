"""
A file with some random scribbles that might be interesting
"""

from collections import Counter
from distutils import core
import itertools
import math
import random
import sys
from tkinter.filedialog import test
from core.mixcolumn_trail_search import find_input_mask, find_output_mask
from core.subbytes import mulp_inv
from core.correlation import corr, func_corr, icorr, ipm16, ipms_with_abs_corr, keyed_func_corr, ocorr, opm16, opms_with_abs_corr
from core.toolbox import I, P8, GF2_8, GF2_8_min_0, bin_, export_table, hex_, join32, signum, split32, subbyte, _subbytes, xor_closure
from core.mmm import MM5_input_masks, MM5_output_masks, core_input_masks, majority_func_for_masks, weighted_majority_func_for_masks
from core.key_classes import MM3_key_class_corr, MM5_class_reps, MM5_key_class_corr, orthogonal_class


def correlation_count():
    opm = 0x53
    ipms = MM5_input_masks(opm)

    def opf(ct):
        return P8(ct & opm)

    res = []
    for key in GF2_8:
        def ipf(pt):
            return 5 - 2 * sum(P8(ipm & (pt ^ key)) for ipm in ipms) > 0

        corr128 = func_corr(ipf, opf)
        res.append(corr128)
    print(Counter(res))


def trail_mulp_inv():
    LUT = [mulp_inv(x) for x in GF2_8]

    # Compute correlations
    res = {}
    for opm in GF2_8_min_0:
        res[opm] = {}

        def opf(ct):
            return P8(ct & opm)
        for ipm in GF2_8_min_0:
            def ipf(pt):
                return P8(pt & ipm)

            corr128 = 128
            for pt in GF2_8:
                corr128 -= ipf(pt) ^ opf(LUT[pt])
            res[opm][ipm] = corr128

    # Print results
    for opm in GF2_8_min_0:
        ipms = list(filter(lambda x: abs(x[1]) >= 16, res[opm].items()))
        print(hex_(opm), [hex_(ipm) for ipm, corr in ipms], ipms[0][1])
        print("    ", [hex_(LUT[ipm]) for ipm, corr in ipms], hex_(LUT[opm]))


def backwards_majority_trail():
    ipm = 0x01
    opms = icorr(ipm)
    masks = [(signum(c), m) for m, c in enumerate(opms) if abs(c) == 16]

    def ipf(pt):
        return P8(pt & ipm)

    results = []
    for k in GF2_8:
        def opf(ct):
            return sum(s*I(P8(m & ct)) for s, m in masks) >= 0

        results.append(keyed_func_corr(ipf, opf, ipk=0, opk=k))

    print(results, Counter(results))


def binom(n, k):
    return math.perm(n, k) / math.factorial(k)


def information_mm3(nr_sboxes):
    p = nr_sboxes
    y = [
        binom(p, i) * 3**(p-i) * 2**(-2*p-1)
        for i in range(0, p+1)
    ]
    y = sum(i * math.log2(i) for i in y)
    return -2*y


def information_mm5(nr_sboxes):
    p = nr_sboxes
    x = 1 - (7/8)**p
    y = [binom(p, i) * binom(p-i, j) * (2 ** (i+2*j-3*p))
         for i in range(0, p+1)
         for j in range(0, p-i+1)]
    y = sum(i * math.log2(i) for i in y)
    return - x * math.log2(x) - y


def j_is_7():
    for opm in GF2_8_min_0:
        ipms = xor_closure(next(core_input_masks(opm)))
        ipms.remove(0)
        assert len(ipms) == 7

        print([corr(ipm, opm) for ipm in ipms])

        # def opf(ct):
        #     return P8(ct & opm)

        # ipf = majority_func_for_masks(ipms)

        # res = [keyed_func_corr(ipf, opf, ipk=k, opk=0) for k in GF2_8]
        # bit = P8(opm & 0b01100011)
        # assert (bit == 1) ^ (-4 in Counter(res)), (hex_(opm), res)
        # print(hex_(opm), bit, Counter(res), res)


def test_multiple_one_single():
    count = 0
    # Iterate over all 8-bit plaintexts
    for pt in GF2_8:

        # Compute sum of 5 terms
        mask_sum = 0
        mask_sum += 1 - 2 * P8(pt & 0xE5)  # (-1)ˆ{E5 T x}
        mask_sum += 1 - 2 * P8(pt & 0x93)  # (-1)ˆ{93 T x}
        mask_sum += 1 - 2 * P8(pt & 0xA9)  # (-1)ˆ{A9 T x}
        mask_sum += 1 - 2 * P8(pt & 0x76)  # (-1)ˆ{76 T x}
        mask_sum += 1 - 2 * P8(pt & 0x4C)  # (-1)ˆ{4C T x}

        # Convert sum to output parity prediction
        if mask_sum > 0:
            prediction = 0
        else:
            prediction = 1

        # Compute output parity
        ct = subbyte(pt)
        output_parity = P8(ct & 0x9C)

        # Loop invariant: ‘count’ stores the number of times the
        # output parity prediction ‘prediction’ was the same as
        # the actual output_parity ‘output_parity’.
        count += 1 ^ prediction ^ output_parity

    # correlation = 2 * (160 / 256) - 1 = 2ˆ-2
    assert count == 160, count


def test_one_multiple_single():
    count = 0
    for pt in GF2_8:
        input_parity = P8(pt & 0x4A)

        ct = subbyte(pt)
        mask_sum = 0
        mask_sum += 1 - 2 * P8(ct & 0xAA)
        mask_sum -= 1 - 2 * P8(ct & 0x58)
        mask_sum += 1 - 2 * P8(ct & 0x59)
        mask_sum -= 1 - 2 * P8(ct & 0xF2)
        mask_sum += 1 - 2 * P8(ct & 0xF3)

        if mask_sum > 0:
            prediction = 0
        else:
            prediction = 1

        count += 1 ^ input_parity ^ prediction
    assert count == 160, count

def removed_masks(opm, th):
    masks = [(corr(m, opm), m) for m in GF2_8_min_0 if abs(corr(m, opm)) > th]
    w1, *_ = next(core_input_masks(opm))
    masks.remove((corr(w1, opm), w1))

    tuples = [(pt, subbyte(pt)) for pt in GF2_8]

    res = {0: [], 1: []}

    def ipf(pt):
        return sum(cor * I(P8(m & pt)) for cor, m in masks)
    def opf(ct):
        return P8(ct & opm)

    for pt, ct in tuples:
        res[opf(ct)].append(ipf(pt))

    for k,v in res.items():
        v.sort()

    assert not set(res[0]).intersection(res[1])



def remove_masks(opm, nr_iterations):
    masks = [(corr(m, opm), m) for m in GF2_8_min_0 if abs(corr(m, opm)) >= 10]
    removed = []

    def try_to_remove(mask):
        masks.remove(mask)

        def ipf(pt):
            return sum(cor * I(P8(m & pt)) for cor, m in masks)

        res = {0: [], 1:[]}
        for pt, ct in zip(GF2_8, _subbytes):
            res[P8(ct & opm)].append(ipf(pt))

        if set(res[0]).intersection(res[1]):
            masks.append(mask)
        else:
            removed.append(mask)


    for __ in range(nr_iterations):
        to_remove = random.choice(masks)
        try_to_remove(to_remove)
    for m in [m for m in reversed(masks)]:
        try_to_remove(m)

    print(opm)
    assert len(removed) == 1
    assert removed[0][1] == next(core_input_masks(opm))[0], (removed, next(core_input_masks(opm))[0])
    # print(len(masks), [hex_(x) for _, x in sorted(removed, key=lambda x: x[1])])
    # print(len(xor_closure([m for c, m in masks])))


def find_fequencies():
    vecs = []
    for opm in GF2_8_min_0:
        if P8(opm & subbyte(0)) == 1:
            continue
        w1, w2, w3 = next(core_input_masks(opm))
        masks = [w1, w2, w3, w1 ^ w2, w1 ^ w3, w2 ^ w3, w1 ^ w2 ^ w3]
        sets = [(corr(m, opm), m) for m in masks]

        res = []
        for pt in GF2_8:
            res.append((P8(w1 & pt), P8(w2 & pt), P8(w3 & pt), P8(opm & subbyte(pt))))
        c = Counter(res)

        print(hex_(opm))
        items = sorted(c.items())
        for k, v in items:
            print(f"  {v:2}x {k}")

        vecs.append(tuple([v for _, v in items]))

    print(Counter(vecs))


def find_subcorrelation_masks(opm=0x01):
    """
    Find masks that yield high correlation when w1 = 1
    """
    w1, w2, w3 = next(core_input_masks(opm))
    tuples = [(pt, subbyte(pt)) for pt in GF2_8 if P8(pt & w1)]


    # Build correlation table only for subpts
    corr64 = []
    for m in GF2_8:
        count = 64
        for pt, ct in tuples:
            count -= P8(m & pt) ^ P8(opm & ct)
        corr64.append(count)

    return [m for m, c in enumerate(corr64)]


def mask_correlation():
    opm = 0x01
    w1, w2, w3 = next(core_input_masks(opm))
    closure = [w1, w2, w3, w1 ^ w2, w1 ^ w3, w2 ^ w3, w1 ^ w2 ^ w3]
    tuples = [(pt, subbyte(pt)) for pt in GF2_8 if P8(w1 & pt)]
    masks = find_subcorrelation_masks(opm)

    gaps = {}
    for w4, w5 in itertools.combinations(masks, 2):
        res = []
        for pt, ct in tuples:
            res.append((P8(w4 & pt), P8(w5 & pt), P8(opm & ct)))

        c = Counter(res)

        gap = 0
        for m in [(0, 0), (0, 1), (1, 0), (1, 1)]:
            c0, c1 = c[(*m, 0)], c[(*m, 1)]
            gap += abs(c0 - c1)
        gaps[(w4, w5)] = gap

    best_gaps = {k:v for k, v in sorted(gaps.items(), key=lambda x: x[1]) if v >= 40}

    for k, v in best_gaps.items():
        print(tuple([hex_(x) for x in k]), v)


def w1_completer():
    opm = 0x01
    w1, *_ = next(core_input_masks(opm))
    tuples = [(pt, subbyte(pt)) for pt in GF2_8]

    counts = {}
    for m2 in GF2_8_min_0:
        res = []
        for pt, ct in tuples:
            res.append((P8(w1 & pt), P8(m2 & pt), P8(opm & ct)))

        c = Counter(res)

        gap = 0
        for m in [(0, 0), (0, 1), (1, 0), (1, 1)]:
            c0, c1 = c[(*m, 0)], c[(*m, 1)]
            gap += abs(c0 - c1)
        counts[hex_(m2)] = -math.log2(2 * int(128 + gap/2) / 256 -1)

    best_gaps = {k:v for k, v in sorted(counts.items(), key=lambda x: x[1])}
    print(best_gaps)


def best_combo_finder():
    opm = 0x01
    tuples = [(pt, subbyte(pt)) for pt in GF2_8]

    counts = {}
    for m1, m2 in itertools.combinations(GF2_8_min_0, 2):
        res = []
        for pt, ct in tuples:
            res.append((P8(m1 & pt), P8(m2 & pt), P8(opm & ct)))

        c = Counter(res)

        gap = 0
        for m in [(0, 0), (0, 1), (1, 0), (1, 1)]:
            c0, c1 = c[(*m, 0)], c[(*m, 1)]
            gap += abs(c0 - c1)
        counts[(m1, m2)] = -math.log2(2 * int(128 + gap/2) / 256 -1)

    best_gaps = {k:v for k, v in sorted(counts.items(), key=lambda x: x[1])}
    print(best_gaps)

def construct_partitioner(opm: int):
    corrs = ocorr(opm)

    division = {}
    for ipm, c in enumerate(corrs):
        division.setdefault(abs(c), [])
        division[abs(c)].append(ipm)
    division = {k:v for k,v in sorted(division.items())}
    # print(division)

    masks = []
    for v in list(division.values())[:3:-1]:
        for x in v:
            if x not in xor_closure(masks):
                masks.append(x)
                break
        else:
            raise ValueError("KAK")
    masks = [(corr(m, opm), m) for m in masks]
    print(masks)

    def ipf(pt):
        return sum(c * I(P8(pt & m)) for c, m in masks)
    def opf(ct):
        return P8(ct & opm)

    tuples = [(pt, subbyte(pt)) for pt in GF2_8]
    res = {0:set(), 1:set()}
    for pt, ct in tuples:
        res[opf(ct)].add(ipf(pt))
    res[0] = set(sorted(res[0]))
    res[1] = set(sorted(res[1]))

    print(res)
    print(sorted(res[0].intersection(res[1])))


def construct_partitioner_v2(opm):
    w1, w2, w3 = next(core_input_masks(opm))
    masks = [w1, w2, w3, w2 ^ w3, 2, 30, 59, 150]
    masks = [(corr(m, opm), m) for m in masks]
    print(masks)

    def ipf(pt):
        return sum(c * I(P8(pt & m)) for c, m in masks)
    def opf(ct):
        return P8(ct & opm)

    tuples = [(pt, subbyte(pt)) for pt in GF2_8]
    res = {0:set(), 1:set()}
    for pt, ct in tuples:
        res[opf(ct)].add(ipf(pt))
    res[0] = set(sorted(res[0]))
    res[1] = set(sorted(res[1]))

    print(res)
    print(sorted(res[0].intersection(res[1])))


def random_construction(opm):
    masks, non_masks = [], [(corr(ipm, opm), ipm) for ipm in GF2_8]

    # Statics
    tuples = [(pt, subbyte(pt)) for pt in GF2_8]

    def opf(ct):
        return P8(ct & opm)

    separable = False
    while not separable:
        # Add new mask
        new_mask = random.choice(non_masks)
        masks.append(new_mask)
        non_masks.remove(new_mask)

        # Create input function
        def ipf(pt):
            return sum(c * I(P8(pt & m)) for c, m in masks)


        # Check separability
        res = {0:set(), 1:set()}
        for pt, ct in tuples:
            res[opf(ct)].add(ipf(pt))

        separable = not res[0].intersection(res[1])
    return masks


def correlation_class_investigation(opm, nr_iters):
    masks = [(corr(ipm, opm), ipm) for ipm in GF2_8 if abs(corr(ipm, opm)) >= 12]

    # Statics
    tuples = [(pt, subbyte(pt)) for pt in GF2_8]
    def opf(ct):
        return P8(ct & opm)

    for _ in range(nr_iters):
        random.shuffle(masks)
        size = random.randint(1, len(masks))

        def ipf(pt):
            return sum(c * I(P8(pt & m)) for c, m in masks[:size])

        res = {0:set(), 1:set()}
        for pt, ct in tuples:
            ipc, opp = ipf(pt), opf(ct)
            res[opp].add(ipc)

        isct = sorted(res[0].intersection(res[1]))
        if not isct:
            print(f"GOT ONE!: {len(masks)}:{masks}")


if __name__ == "__main__":
    # find_subcorrelations()
    # mask_correlation()
    # w1_completer()

    # construct_partitioner_v2(0x01)
    # results = []
    # for _ in range(100):
    #     results.append(random_construction(0x01))
    # results = sorted(results, key=lambda x: len(x))
    # print(f"RESULT: {len(results[0])}", results[0])

    correlation_class_investigation(0x01, 10000)

    # for opm in GF2_8_min_0:
    #     remove_masks(opm, 0)

    # print(Counter(ocorr(0x03)))

    pass
