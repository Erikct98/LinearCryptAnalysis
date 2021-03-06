"""
This library provides a host of small functions that
are useful in many situations.
"""
import random
from itertools import chain, combinations
from typing import Any, List, Tuple

Table = List[List[Any]]

Corr128 = int

GF2_8 = range(256)  # F_2^8
GF2_8_min_0 = range(1, 256)  # F_2^8 \ {0}

_subbytes = [
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,
    0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
    0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,
    0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,
    0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
    0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,
    0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,
    0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
    0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,
    0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,
    0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
    0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,
    0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,
    0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
    0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,
    0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,
    0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
]


def subbyte(x: int) -> int:
    """
    Apply subbyte transformation to 8-bit word `x`.
    """
    return _subbytes[x]


def inv_subbyte(x: int) -> int:
    """
    Apply the inverse subbyte transformation to 8-bit word `x`.
    """
    return _subbytes.index(x)


def I(x: int) -> int:
    """
    Compute (-1)^x. Assumes x in {0,1}
    """
    return 1-(x << 1)


def J(x):
    """
    Compute log_{-1}(x), where x is assumed to be in {-1, 1}.
    """
    return (1 - x) >> 1


def F(x: int, i: int) -> int:
    """
    Get `i`th bit of x, with `0`th bit being the LSB.
    """
    return (x >> i) & 0x1


def P8(v: int) -> int:
    """
    Compute Parity of 8-bit word `v`.
    """
    v ^= v >> 4
    v &= 0xF
    return (0x6996 >> v) & 0x1


def P32(v: int) -> int:
    """
    Compute Parity of 16-bit word `v`.
    """
    v ^= v >> 16
    v ^= v >> 8
    v ^= v >> 4
    v &= 0xF
    return (0x6996 >> v) & 0x1


def bit_count(i: int) -> int:
    """
    Return the number of 1-bits in word `i`.
    """
    return f"{i:b}".count("1")


def transpose(table: List[List[Any]]) -> List[List[Any]]:
    """
    Return the transpose of `table`.
    """
    return [[r[idx] for r in table] for idx, _ in enumerate(table)]


def cylesh8(x: int, steps: int) -> int:
    """
    CYclic LEft SHift 8-bit word by `steps` bits.
    """
    return (x << steps ^ x >> 8 - steps) & 0xFF


def cylesh32(x: int, steps: int) -> int:
    """
    CYclic LEft SHift 32-bit word by `steps` bits.
    """
    return ((x << steps) ^ (x >> (32 - steps))) & 0xFFFFFFFF


def export_table(table: Table) -> None:
    """
    Write a fancy representation of `table` to file with name `fname`.
    """
    print(f'    {" ".join(f"  {x:02X}" for x, _ in enumerate(table[0]))} ')
    print('-' * (5 * len(table) + 3))
    for idx, row in enumerate(table):
        print(f'{idx:02X} [{",".join(f"{x:4}" for x in row)}]')


def rand32() -> int:
    return random.randint(0, 0xFFFFFFFF)


def signum(x) -> int:
    """
    Return the sign(um) of x
    """
    return int(x / abs(x)) if x != 0 else 1


def int_to_hex(*x: int) -> int:
    """
    Convert int-interable to list of integers in hex-notation.
    """
    return [f"0x{elt:02X}" for elt in x]


def xorsum(lst: List[int]) -> int:
    """
    Compute the sum of the elements in `lst` where we use the XOR operator.
    """
    elt = 0
    for x in lst:
        elt ^= x
    return elt


def orsum(lst: List[int]) -> int:
    """
    Compute the sum of the elements in `lst` where we use the OR operator.
    """
    elt = 0
    for x in lst:
        elt |= x
    return elt


def powerset(iterable):
    """
    powerset([1,2,3]) --> () (1,) (2,) (3,) (1,2) (1,3) (2,3) (1,2,3)
    """
    s = list(iterable)
    return chain.from_iterable(combinations(s, r) for r in range(len(s)+1))


def xor_closure(basis: List[int]) -> List[int]:
    """
    Compute the closure of `basis`, based on the xor-operator.
    """
    return set(xorsum(elts) for elts in powerset(basis))


def split32(word: int) -> Tuple[int, int, int, int]:
    """
    Split a 32-bit word into 4 8-bit words.
    """
    return tuple([word >> (24 - 8 * i) & 0xFF for i in range(4)])


def join32(*words: int) -> int:
    """
    Join 4 8-bit words into a 32-bit word.
    """
    return xorsum([w << (24 - 8 * i) for i, w in enumerate(words)])


def hex_(x: int, width=2):
    return "0x" + str.rjust(f"{x:X}", width, "0")


def bin_(x: int, width=8):
    return "0b" + str.rjust(f"{x:b}", width, "0")
