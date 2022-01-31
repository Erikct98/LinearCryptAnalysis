"""
This library provides tools that are useful in trailing through AES in case
of non-zero keys and extracting keybits in the process.
"""
from correlation import keyed_func_corr
from linearization import keyed_linearization_for_ipm
from toolbox import P8, GF2_8


def compute_correlations_wo_key():
    results = [list() for _ in GF2_8]
    for ipm in GF2_8:
        for key in GF2_8:
            def ipf(pt): return P8(pt & ipm)
            opf = keyed_linearization_for_ipm(ipm, 0x01)
            results[ipm].append(keyed_func_corr(ipf, opf, 0x0, key))
        print(results[ipm])
    return results

if __name__ == "__main__":
    print(compute_correlations_wo_key())
