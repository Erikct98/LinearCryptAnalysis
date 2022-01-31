"""
This library assists in working with the SubBytes step in the AES algorithm.
"""
from typing import List
from toolbox import P8, GF2_8, cylesh8, Table


def mulp(x: int, y: int) -> int:
    """
    XOR-multiply `x` and `y` xor-mod 283
    Note: 283 is the AES modulo.
    """
    # mulp
    res = 0
    while x:
        if x & 1:
            res ^= y
        y <<= 1
        x >>= 1

    # mod
    val = 283 << 8
    for _ in range(8):
        val >>= 1
        if (res ^ val) < res:
            res ^= val
    return res


def mulp_inv(x: int) -> int:
    """
    Computes multiplicative inverse of x over the 283 modulo.
    Note: 0 is mapped to itself.
    """
    if x == 0:
        return 0
    for j in GF2_8:
        res = mulp(x, j)
        if res == 1:
            return j
    raise ValueError(x)


def transform(x: int) -> int:
    """
    Apply the linear transformation of the AES SubBytes step to 8-bit word `x`.
    """
    return x ^ cylesh8(x, 1) ^ cylesh8(x, 2) \
             ^ cylesh8(x, 3) ^ cylesh8(x, 4) ^ 0x63


def compute_AES_subbytes() -> List[int]:
    """
    Compute AES SubBytes lookup table for 8-bit words.
    """
    return [transform(mulp_inv(i)) for i in GF2_8]


def compute_subbytes_LAT() -> Table:
    """
    Compute AES SubBytes correlation lookup table for 8-bit input and output
    masks.
    """
    sbox = compute_AES_subbytes()
    LAT = [[128]*256 for _ in GF2_8]
    for ipm in GF2_8:
        for opm in GF2_8:
            LAT[ipm][opm] -= sum(P8(ipm & pt ^ opm & sbox[pt]) for pt in GF2_8)
    return LAT
