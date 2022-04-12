"""
Analyses the results from the MonteCarlo tests.
"""

import json
import math
from correlation import keyed_func_corr
from toolbox import GF2_8, I, split32, P8
from mixcolumn_trail_search import find_input_mask
from typing import Dict
from mmm import MM5_input_masks, core_input_masks_v2, majority_func_for_masks

FNAME = "../AES/montecarloresults3.json"

IPM = find_input_mask(0x2D000000)

def keycategory(opm, key):
    """
    As for now, we assume to work with opm 0x01.
    """
    assert opm == 0x01
    ipms = split32(IPM)
    keys = split32(key)
    categories = []
    for ipm, key in zip(ipms, keys):
        masks = next(core_input_masks_v2(ipm))
        ips = [P8(m & key) for m in masks]
        categories.append("".join(str(ip) for ip in ips))

    return " ".join(categories)

def expected_correlation(guess, actual):
    opms = split32(IPM)
    guess_keys = split32(guess)
    actual_keys = split32(actual)


    corrs = []
    for opm, kg, ka in zip(opms, guess_keys, actual_keys):
        masks = MM5_input_masks(opm)

        def ipf(pt):
            return sum(I(P8(m & (pt ^ kg))) for m in masks) < 0
        def opf(ct):
            return P8(ct & opm)

        corr128 = keyed_func_corr(ipf, opf, ipk=ka, opk=0)
        corrs.append(corr128 / 128)
    print(corrs)

    return math.prod(corrs) / 8

def analyse():
    print("ANALYSING", FNAME)
    with open(FNAME, 'r') as fp:
        data = json.load(fp)

    sample_size = data['samplesize']
    print("sample size =", sample_size)

    opm = int(data['opm'][2:], 16)
    print(f"opm: 0x{opm:02X}")

    trails = data['trails']
    for tid, trail in enumerate(trails):
        trail_key = int(trail['key'][2:], 16)

        print(f"TRAIL {tid}, with trail_key = 0x{trail_key:08X}")

        counts : Dict[str, int] = trail['counts']
        counts = {
            int(k[2:], 16): v for k, v in counts.items()
        }

        for kg, cnt in counts.items():
            corr = 2 * cnt / sample_size - 1
            corrlog = math.log2(abs(corr)) if corr != 0 else - math.inf
            expectedcorr = expected_correlation(kg, trail_key)
            expectedcorrlog = math.log2(abs(expectedcorr)) if expectedcorr != 0 else - math.inf
            # expectedcorr = math.log2(abs())
            print(f"  kg=0x{kg:08X}, cat={keycategory(0x01, kg)}, corr={corrlog:.2f}, exp={expectedcorrlog:.2f}")


        # print(tid, trail, counts)
        # print(tid, trail, f"0x{trail_key:08X}")

if __name__ == "__main__":
    analyse()