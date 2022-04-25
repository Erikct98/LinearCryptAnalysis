from collections import Counter
from typing import Dict, List
import math


def subsetsum(nr_items, sum):
    def subsetsum_(nr_items: int, sum: int, stem: List):
        if nr_items == 1:
            stem.append(sum)
            yield stem
            stem.pop()
        else:
            for i in range(sum + 1):
                stem.append(i)
                yield from subsetsum_(nr_items-1, sum-i, stem)
                stem.pop()
    yield from subsetsum_(nr_items, sum, [])


def expected_information(nr_sboxes: int, weights_exps: Dict[int, int]):
    """
    Returns the expected amount of information (in bits).
    :param nr_sboxes: number of sboxes in play.
    :param weights_exps: mapping from corr128 to probability of this occurring.
    """
    assert all(map(lambda x: x[1] == 1, Counter(map(lambda k: abs(k), weights_exps)).items()))

    # Computed
    weights = list(weights_exps)
    total_nr_permutations = math.factorial(nr_sboxes)

    # Computation
    exp_info = 0
    for vals in subsetsum(len(weights), nr_sboxes):
        line = {k: v for k, v in zip(weights, vals)}

        # Probability of one of these tuples occurring
        prob_exp = sum(weights_exps[k] * v for k, v in zip(weights, vals))

        # Permutations
        nr_equiv_classes = math.prod(math.factorial(x) for x in line.values())
        permutations = int(total_nr_permutations / nr_equiv_classes)
        perm_exp = math.log2(permutations)

        # Correlation
        corr = abs(math.prod(k ** v for k, v in line.items()))
        corr_exp = -math.inf if corr == 0 else math.log2(corr) - 7 * nr_sboxes

        # Compute information gain
        if corr_exp >= -64:
            # if this correlation can be detected
            bits_of_info = - (prob_exp + perm_exp)
            prob_of_info = 2 ** (prob_exp + perm_exp)
            exp_info += bits_of_info * prob_of_info

    return exp_info


if __name__ == "__main__":
    pass
