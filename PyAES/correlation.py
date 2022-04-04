"""
This library assists in working with the Linear Approximation Table
of the AES sbox.
"""

from typing import Callable, List
from toolbox import GF2_8, subbyte
from LAT import LAT


def corr(ipm: int, opm: int) -> int:
    """
    :returns: 128 * correlation between the input mask `ipm` and output mask
    `opm` through the AES sbox
    """
    return LAT[ipm][opm]


def icorr(ipm: int) -> List[int]:
    """
    Returns list of correlations, where the correlation at index `0 <= i < 256`
    is the correlation between input mask `ipm` and output mask `i`.
    """
    return [LAT[ipm][opm] for opm in GF2_8]


def ocorr(opm: int) -> List[int]:
    """
    Returns list of correlations, where the correlation at index `0<=i<256`
    is the correlation between input mask `i` and output mask `opm`.
    """
    return [LAT[ipm][opm] for ipm in GF2_8]


def ipms_with_abs_corr(opm: int, corr128: int) -> List[int]:
    """
    Returns list of input masks that have an absolute correlation * 128
    of `corr128` with opm `opm`.
    """
    return [i for i, v in enumerate(ocorr(opm)) if abs(v) == corr128]


def opms_with_abs_corr(ipm: int, corr128: int) -> List[int]:
    """
    Returns list of output masks that have an absolute correlation * 128
    of `corr128` with ipm `ipm`.
    """
    return [i for i, v in enumerate(icorr(ipm)) if abs(v) == corr128]


def ipm16(opm: int) -> int:
    """
    Returns an input mask that achieves +/- 2^-3 correlation over AES sbox
    with output mask `opm`.
    """
    if opm == 0: return 0
    return ipms_with_abs_corr(opm, 16)[0]


def opm16(ipm: int) -> int:
    """
    Returns an output mask that achieves +/- 2^-3 correlation over AES sbox
    with input mask `ipm`.
    """
    if ipm == 0: return 0
    return opms_with_abs_corr(ipm, 16)[0]


def func_corr(ipf: Callable, opf: Callable) -> int:
    """
    Computes the correlation between input function `ipf` and
    output function `opf` over the AES sbox.
    :returns: 128 * correlation between `ipf(pt)` and `opf(ct)`
    """
    count = 128
    for pt in GF2_8:
        ct = subbyte(pt)
        count -= ipf(pt) ^ opf(ct)
    return count


def keyed_func_corr(ipf: Callable, opf: Callable, ipk: int, opk: int) -> List[int]:
    """
    Return correlation between input function `ipf` and output function `opf`
    through `ipk`-XOR, SubBytes and `opk`-XOR.
    :param ipf: function on input side
    :param opf: function on output side
    :param ipk: key at input side
    :param opk: key at output side
    :returns: 128*correlation
    """
    corr128 = 128
    for pt in GF2_8:
        ct = opk ^ subbyte(pt ^ ipk)
        corr128 -= ipf(pt) ^ opf(ct)
    return corr128


if __name__ == "__main__":
    print(corr(0x1, 0x3))
