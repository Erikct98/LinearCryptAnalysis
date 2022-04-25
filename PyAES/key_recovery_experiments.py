"""
This library provides tools that are useful in trailing through AES in case
of non-zero keys and extracting keybits in the process.
"""
from core.correlation import keyed_func_corr
from core.toolbox import I, J, P8, GF2_8, bit_count
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

if __name__ == "__main__":
    pass
