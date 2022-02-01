"""
This library contains tools used in the MixColumn step of the AES encryption scheme.
"""

from subbytes import mulp


def mixcolumn(x: int) -> int:
    """
    Apply MixColumn step to column `x`.
    :returns: result after applying MixColumn step to `x`.
    """
    x_ = [x >> 24 & 0xFF, x >> 16 & 0xFF, x >> 8 & 0xFF, x & 0xFF]
    y0 = mulp(x_[0], 2) ^ mulp(x_[1], 3) ^ mulp(x_[2], 1) ^ mulp(x_[3], 1)
    y1 = mulp(x_[0], 1) ^ mulp(x_[1], 2) ^ mulp(x_[2], 3) ^ mulp(x_[3], 1)
    y2 = mulp(x_[0], 1) ^ mulp(x_[1], 1) ^ mulp(x_[2], 2) ^ mulp(x_[3], 3)
    y3 = mulp(x_[0], 3) ^ mulp(x_[1], 1) ^ mulp(x_[2], 1) ^ mulp(x_[3], 2)
    return y0 << 24 ^ y1 << 16 ^ y2 << 8 ^ y3


def test_mixcolumn():
    assert mixcolumn(0x007f0000) == 0x81fe7f7f
    assert mixcolumn(0x702f4f6f) == 0xb190702e


if __name__ == "__main__":
    pass
