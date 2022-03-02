"""
In case of unknown key, the support of the correlation under guessing
the zero-key is [0, +- 1/32, +- 2/32, +- 3/32, +- 4/32, 32/32].

In this module, we compute the probability of a certain correlation occurring
when guessing 8 key-bytes in parallel.
"""

import math
from itertools import permutations
from typing import Dict, Tuple


def subsets(elts: int, sum: int) -> Tuple[int]:
    """
    Generator that iterates over all sets of `elts` non-negative integers
    that sum up to `sum`.
    """
    if elts == 1:
        yield [sum]
    else:
        for i in range(sum + 1):
            for x in subsets(elts-1, sum-i):
                yield tuple([i, *x])


def probability_mapping(sbox_count: int) -> Dict[int, int]:
    """
    Returns mapping of `2^40 * corr` to `2^64 * prob` the probability that
    correlation when guessing `sbox_count` key-bytes simultaneously.
    """
    mapping = {}
    correlations = [0, 1, 2, 3, 4, 32]
    ratios = [32, 84, 74, 52, 13, 1]
    for ss in subsets(6, sbox_count):
        # Compute correlation
        corr = 1
        for cnt, w in zip(ss, correlations):
            corr *= w ** cnt

        # Compute probability of occuring
        lineup = [x for idx, cnt in enumerate(ss) for x in [idx] * cnt]
        nr_permutations = len(set(permutations(lineup)))
        prob = 1
        for cnt, p in zip(ss, ratios):
            prob *= p ** cnt
        prob *= nr_permutations

        mapping.setdefault(corr, 0)
        mapping[corr] += prob
    return mapping


def plot_PDF(sbox_count: int) -> None:
    """
    Create a log-log plot of the PDF when guessing the key for `sbox_count`
    sboxes simultaneously
    """
    mapping = probability_mapping(8)

    def log2(x):
        if x == 0:
            return -100
        return math.log2(x)

    mapping = {log2(k) : log2(v) for k,v in mapping.items()}

    from matplotlib import pyplot as plt

    fig, ax = plt.subplots(figsize=(10, 7))
    res = mapping.values()
    ax.hist(list(mapping.keys()), weights=list(mapping.values()), bins=600)
    plt.show()


if __name__ == "__main__":
    pass