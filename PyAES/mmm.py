"""
Library used to compute information surrounding the
Multiple Mask Method.
"""

from itertools import combinations
from typing import List
from toolbox import I, J, P8, signum
from correlation import func_corr, ocorr


def MM5_input_masks(opm: int) -> List[int]:
    """
    Returns 5 input masks that give a correlation of 2^-2
    when exploited by the 5 Mask Method.
    """
    return [i for i, v in enumerate(ocorr(opm)) if abs(v) == 16]


def MM3_input_masks(opm: int) -> List[int]:
    """
    Returns 3 input masks that give a correlation of 7/32
    when exploited by the 3 Mask Method.
    """
    m5 = MM5_input_masks(opm)

    def opf(ct: int):
        return P8(ct & opm)

    for combo in combinations(m5, 3):
        def ipf(pt: int):
            return J(signum(sum(I(P8(pt & ipm)) for ipm in combo)))

        corr128 = func_corr(ipf, opf)
        if abs(corr128) == 28:
            return combo


if __name__ == "__main__":
    x = [[f"0x{x:02X}"  for x in MM5_input_masks(opm)] for opm in range(1, 256)]
    print(x)
    pass
