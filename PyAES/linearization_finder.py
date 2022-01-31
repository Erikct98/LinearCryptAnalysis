from typing import List, Tuple
from toolbox import sbox, GF2_8, F, I, J, P8
from LAT import LAT
import random


def create_linearization(linearization_terms: List[Tuple[int, int, int]], term_name) -> str:
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


def print_linearization_for_opm(opm: int, nr_terms=5):
    mapping = get_opm_mapping(opm)
    mapping.sort(reverse=True)

    # Create linearization "code"
    linearization_terms = mapping[:nr_terms]
    optimals = {}
    for i in range(1, 2 ** nr_terms):
        used_terms = [linearization_terms[idx]
                      for idx, i in enumerate(reversed(f'{i:016b}')) if i == "1"]
        formula = create_linearization(used_terms, "pt")
        # print(formula)

        # Compute correlation
        remainder = "P(ct & opm)"
        count = compute_correlation(formula, remainder, opm=opm)
        optimals.setdefault(count, i)
        corr = 2 * (count / 256) - 1
        print(f"{i=}, {count=}, {corr=}")


def random_find_linearization_for_opm(opm: int, nr_terms=5, pool_size=256, iterations=10):
    assert nr_terms <= pool_size
    mapping = get_opm_mapping(opm)
    mapping.sort(reverse=True)

    remainder = "P(ct & opm)"
    pool = mapping[:pool_size]
    best = (128, 0, -1, [0])
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


def print_linearization_for_ipm(ipm: int, nr_terms=5):
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
        tally = (F(pt, 3) | F(pt, 7)) ^ (F(pt, 5) | F(pt, 1))
        # tally = - I(F(pt, 3) ^ F(pt, 5) ^ F(pt, 6))\
        #       - I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 6) ^ F(pt, 7))\
        #       - I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 7))\
        #       + I(F(pt, 0) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 7))\
        #       + I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 5))
        in_parity = tally

        # in_parity = (1- F(pt, 1)) * (1-F(pt, 6)) * (F(pt, 3) ^ F(pt, 7))

        # in_parity = ((a1 + a2 + a3 + a4) // 2) >= 0
        # print(in_parity, tally)

        out_parity = P8(sbox[pt] & 0x7A)
        count += (in_parity ^ out_parity) & 0x1
    print(count)


if __name__ == "__main__":
    random_find_linearization_for_opm(0x01, 5, 57, 2048)
    # test_linearization()


# print("IPM")
# for i in range(1, 60):

# print("OPM")
# for i in range(1, 60):
#     print_linearization_for_opm(0x7A, i)
