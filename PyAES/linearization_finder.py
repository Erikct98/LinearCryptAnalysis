from core.correlation import corr, func_corr
from core.toolbox import GF2_8, P8
import random
from linear_function import LinearFunction


def get_ipm_mapping(ipm: int):
    """
    Return mapping from opm to correlation with `ipm`.
    Sorted by abs(corr) in descending order.
    """
    mapping = {opm: corr(ipm, opm) for opm in GF2_8}
    mapping = dict(
        sorted(mapping.items(), key=lambda x: abs(x[1]), reverse=True))
    return mapping


def get_opm_mapping(opm: int):
    """
    Return mapping from ipm to correlation with `opm`.
    Sorted by abs(corr) in descending order.
    """
    mapping = {ipm: corr(ipm, opm) for ipm in GF2_8}
    mapping = dict(
        sorted(mapping.items(), key=lambda x: abs(x[1]), reverse=True))
    return mapping


def print_linearization_for_opm(opm: int, nr_terms=5):
    """
    Return correlation of sum of the `nr_terms` input mask terms
    with highest individual correlation with output mask `opm`.
    """
    # Get `nr_terms` terms with highest individual correlation
    mapping = get_opm_mapping(opm)
    terms = dict(list(mapping.items())[:nr_terms])

    # Create input and output function
    ipf = LinearFunction(dict(terms))
    def opf(ct): return P8(ct & opm)

    return func_corr(ipf, opf)


def print_linearization_for_ipm(ipm: int, nr_terms=5):
    """
    Return correlation of sum of the `nr_terms` output mask terms
    with highest individual correlation with input mask `ipm`.
    """
    # Get `nr_terms` terms with highest individual correlation
    mapping = get_ipm_mapping(ipm)
    terms = dict(list(mapping.items())[:nr_terms])

    # Create input and output function
    def ipf(pt): return P8(pt & ipm)
    opf = LinearFunction(terms)

    return func_corr(ipf, opf)


def random_find_linearization_for_opm(opm: int, nr_terms=5, pool_size=256, iterations=10):
    """
    By means of random searching, search for combination of `nr_terms` mask
    in the top `pool_size` terms with highest individual correlation that have
    greatest correlation with output mask `opm`.
    The best over `nr_iterations` iterations is returned.
    """
    assert nr_terms <= pool_size
    mapping = get_opm_mapping(opm)
    pool = list(mapping.items())[:pool_size]

    def opf(ct): return P8(ct & opm)

    best = (128, 0, -1, [0])
    for i in range(iterations):
        # Generate random formula
        random.shuffle(pool)

        terms = dict(pool[:nr_terms])
        ipf = LinearFunction(terms)

        # Compute correlation
        corr128 = func_corr(ipf, opf)
        corr = corr128 / 128
        if abs(corr) > abs(best[1]):
            best = (corr128, corr, i, str(ipf), terms)
    return best


if __name__ == "__main__":
    # print(print_linearization_for_ipm(0x7A, nr_terms=5))
    print(random_find_linearization_for_opm(0x01, 10, 57, 2048))
