"""
A file with some random scribbles that might be interesting
"""

from core.mixcolumn_trail_search import find_input_mask, find_output_mask
from core.subbytes import mulp_inv
from core.correlation import corr, func_corr, keyed_func_corr
from core.toolbox import P8, GF2_8, GF2_8_min_0, export_table, hex_, join32, split32, subbyte
from core.mmm import MM5_input_masks, MM5_output_masks, core_input_masks, majority_func_for_masks
from core.key_classes import MM5_class_reps, orthogonal_class
from typing import Counter, List


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

if __name__ == "__main__":
    # for x in subsetsum(4, 20):
    #     print(x)
    pass
