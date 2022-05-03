"""
This library provides tools that are useful in trailing through AES in case
of non-zero keys and extracting keybits in the process.
"""
import random
from core.mmm import core_input_masks
from core.correlation import corr, keyed_func_corr
from core.toolbox import I, J, P8, GF2_8, bit_count, signum, subbyte
from core.LAT import LAT
from linearization import keyed_linearization_for_ipm
from collections import Counter


def compute_correlations_wo_key(key_guess):
    results = [list() for _ in GF2_8]
    for ipm in GF2_8:
        for key in GF2_8:
            def ipf(pt): return P8(pt & ipm)
            opf = keyed_linearization_for_ipm(ipm, key_guess)
            results[ipm].append(keyed_func_corr(ipf, opf, 0x0, key))
        print(results[ipm], Counter(results[ipm]))
    return results

def KAT(guess: int, key:int, opm:int) -> int:
    """
    Key Approximation Table
    :param guess: guessed key
    :param key: actual encryption key
    :param opm: output mask.
    :returns: correlation between guess and key for opm.
    """
    GAT = [LAT[ipm][opm] * I(P8(ipm & guess)) for ipm in GF2_8]
    KAT = [LAT[ipm][opm] * I(P8(ipm & key)) for ipm in GF2_8]
    corr128 = 128
    for pt in GF2_8:
        exp_par = J(sum(GAT[ipm] * I(P8(ipm & pt)) for ipm in GF2_8) >> 7)
        act_par = J(sum(KAT[ipm] * I(P8(ipm & pt)) for ipm in GF2_8) >> 7)
        corr128 -= exp_par ^ act_par
    return corr128


def compute_key_byte(a):
    total = sum(map(lambda x: abs(x[1]), a))
    counts = [sum(map(lambda x : int((f'{x[0]:08b}')[i]) * x[1], a)) - total/2 for i in range(8)]
    print(counts)
    return int(''.join([str(int(counts[i] >= 0)) for i in range(8)]), 2)

def analyse_KAT():
    results = [KAT(guess, 0xfa, 0x01) for guess in GF2_8]
    cors = set(x for x in results)
    for cor in cors:
        print(f'{cor:4}: {sorted([(f"{i:08b}", bit_count(i)) for i in GF2_8 if results[i] == cor], key=lambda x: x[1])}')


LUT = {
    (1, 1, 1, -1): (0, 0, 0),
    (1, 1, -1, 1): (0, 0, 1),
    (1, -1, 1, 1): (0, 1, 0),
    (-1, 1, 1, 1): (0, 1, 1),
    (1, -1, -1, -1): (1, 0, 0),
    (-1, 1, -1, -1): (1, 0, 1),
    (-1, -1, 1, -1): (1, 1, 0),
    (-1, -1, -1, 1): (1, 1, 1)
}

def solve_system(vectors):
    # print("======")
    c = Counter(vectors)
    test_vectors = [(0, 0), (0, 1), (1, 0), (1, 1)]

    # Extract geq guesses and magnitudes
    guesses = []
    for vector in test_vectors:
        x, y = c.get((0, *vector, 0), 0), c.get((0, *vector, 1), 0)
        guesses.append(x - y)
    # print("guesses:", guesses)

    # # CAN WE INVOLVE THE ENTRY WITH THE GREATEST CONFIDENCE?
    # # Extreme entry
    # ee = max([(i,v) for i, v in enumerate(guesses)], key=lambda x: abs(x[1]))
    # print("extreme entry:", ee)
    # signs = (0, *test_vectors[ee[0]])
    # if ee[1] < 0:
    #     signs = tuple(sign ^ 1 for sign in signs)
    # return signs

    # See if this does the trick
    signs = [signum(x) for x in guesses]
    # print("signs before", signs)

    if abs(sum(signs)) != 2:
        # See which guess has to be flipped.
        m = sorted(guesses, key=lambda x: abs(x))[0]
        indexes = [(i, x) for i, x in enumerate(guesses) if x == m]
        if len(indexes) == 1:
            idx = guesses.index(m)
        else:
            # Better tiebreaker.
            v = max(indexes, key=lambda x: abs(guesses[x[0]]))[0]
            idx = indexes.index((v, m))

        signs[idx] = signs[idx] * -1

    # print("signs after ", signs)

    return LUT[tuple(signs)]


def linear_system_solving_esque(ss: int, exps):
    """
    :param ss: sample size
    :param exps: nr experiments
    """
    opm = 0x01
    ipms = list(next(core_input_masks(opm)))
    init_signs = [int(.5 - .5 * signum(corr(ipm, opm))) for ipm in ipms]

    off_by = [0, 0, 0, 0]
    for _ in range(exps):
        # Select key
        key = random.choice(GF2_8)
        # key = 1
        key_parities = tuple(P8(key & ipm) for ipm in ipms)

        # Create data
        pts = [random.choice(GF2_8) for _ in range(ss)]
        # pts = GF2_8
        cts = [subbyte(p ^ key) for p in pts]

        # Compute equations
        res = []
        for pt, ct in zip(pts, cts):
            ipv = tuple(P8(pt & ipm) for ipm in ipms)
            op = P8(ct & opm)

            # Account for non-positive coefficients
            ipv = tuple(i ^ j for i, j in zip(ipv, init_signs))

            # Enforce ipv starts with 0
            if ipv[0]:
                ipv = tuple(i ^ 1 for i in ipv)
                op = op ^ 1
            res.append((*ipv, op))

        # Indicate expected key parities
        exp_key_parities = solve_system(res)
        # print("guessed parities:", exp_key_parities)
        # print("parities:", key_parities)

        # Verify expectations
        distance = sum(i ^ j for i, j in zip(key_parities, exp_key_parities))
        off_by[distance] += 1

    print("\nRESULTS:")
    for i, x in enumerate(off_by):
        print(f"off by {i}: {x} / {exps} = {x / exps * 100:.2f}%")


if __name__ == "__main__":
    linear_system_solving_esque(16, 65536)
