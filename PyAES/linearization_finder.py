from typing import List, Tuple
from LAT import LAT
import random

GF2_8 = range(256)

sbox = [
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

def F(x, i):
    return (x >> i) & 0x1

def I(x):
    return 1-(x<<1)

def J(x):
    return (1 - x) >> 1

def P(v):
    v ^= v >> 4
    v &= 0xF
    return (0x6996 >> v) & 0x1

def nr_bits(x):
    return []

def create_linearization(linearization_terms: List[Tuple[int, int, int]], term_name)-> str:
    return "(" + " + ".join(
        [f"{count:3} * I({' ^ '.join([f'F({term_name}, {i})' for i, elt in enumerate(reversed(f'{ipm:08b}')) if elt == '1'])})"
        for _, count, ipm in linearization_terms]) + ") >= 0"

def compute_correlation(formula1, formula2, ipm=None, opm=None):
    count = 0
    for pt in GF2_8:
        ct = sbox[pt]
        in_parity = eval(formula1)
        out_parity = eval(formula2)
        count += in_parity ^ out_parity
    # print(f"count: {count} / 256  -> correlation: {(2 * (count / 256) - 1)}")
    return count

def get_opm_mapping(opm: int):
# Sort input masks based on correlation
    mapping = []
    for ipm in GF2_8:
        count = LAT[ipm][opm]
        mapping.append((abs(count), count, ipm))
    return mapping

def print_linearization_for_opm(opm: int, nr_terms = 5):
    mapping = get_opm_mapping(opm)
    mapping.sort(reverse=True)

    # Create linearization "code"
    linearization_terms = mapping[:nr_terms]
    optimals = {}
    for i in range(1, 2 ** nr_terms):
        used_terms = [linearization_terms[idx] for idx, i in enumerate(reversed(f'{i:016b}')) if i == "1"]
        formula = create_linearization(used_terms, "pt")
        # print(formula)

        # Compute correlation
        remainder = "P(ct & opm)"
        count = compute_correlation(formula, remainder, opm=opm)
        optimals.setdefault(count, i)
        corr = 2 * (count / 256) - 1
        print(f"{i=}, {count=}, {corr=}")


def random_find_linearization_for_opm(opm: int, nr_terms =5, pool_size=256, iterations=10):
    assert nr_terms <= pool_size
    mapping = get_opm_mapping(opm)
    mapping.sort(reverse=True)

    remainder = "P(ct & opm)"
    pool = mapping[:pool_size]
    best = (128,0,-1, [0])
    for i in range(iterations):
        # Generate random formula
        random.shuffle(pool)
        terms = pool[:nr_terms]
        formula = create_linearization(terms, "pt")

        # Compute correlation
        count = compute_correlation(formula, remainder, opm=opm)
        corr = 2 * (count / 256) - 1
        if abs(corr) > abs(best[1]):
            best = (count, corr, i, formula, terms)
    print(best)


def print_linearization_for_ipm(ipm: int, nr_terms = 5):
    # Sort output masks based on correlation
    mapping = []
    for opm in GF2_8:
        count = LAT[ipm][opm]
        mapping.append((abs(count), count, opm))
    mapping.sort(reverse=True)

    # Create linearization "code"
    linearization_terms = mapping[:nr_terms]
    formula = create_linearization(linearization_terms, "ct")
    # print(formula)

    # Compute correlation
    remainder = "P(pt & ipm)"
    compute_correlation(formula, remainder, ipm=ipm)



def test_linearization():
    count = 0
    for pt in GF2_8:
        tally = - I(F(pt, 3) ^ F(pt, 5) ^ F(pt, 6))\
              - I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 6) ^ F(pt, 7))\
              - I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 7))\
              + I(F(pt, 0) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 7))\
              + I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 5))
        in_parity = tally >= 0

        # in_parity = (1- F(pt, 1)) * (1-F(pt, 6)) * (F(pt, 3) ^ F(pt, 7))

        # in_parity = ((a1 + a2 + a3 + a4) // 2) >= 0
        # print(in_parity, tally)

        out_parity = P(sbox[pt] & 0x7A)
        count += (in_parity ^ out_parity) & 0x1
    print(count)

if __name__ == "__main__":
    random_find_linearization_for_opm(0x7A, 20, 57, 2048)
    # test_linearization()


# print("IPM")
# for i in range(1, 60):
    
# print("OPM")
# for i in range(1, 60):
#     print_linearization_for_opm(0x7A, i)
