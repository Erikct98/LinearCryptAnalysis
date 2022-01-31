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
    Returns list of correlations, where the correlation at index `0<=i<256`
    is the correlation between input mask `ipm` and output mask `i`.
    """
    return [LAT[ipm][opm] for opm in GF2_8]


def ocorr(opm: int) -> List[int]:
    """
    Returns list of correlations, where the correlation at index `0<=i<256`
    is the correlation between input mask `i` and output mask `opm`.
    """
    return [LAT[ipm][opm] for ipm in GF2_8]


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
    through `key`-XOR and SubBytes.
    :param ipf: function on input side
    :param opf: function at output side
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
