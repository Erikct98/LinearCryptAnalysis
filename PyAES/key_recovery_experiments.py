"""
This library provides tools that are useful in trailing through AES in case
of non-zero keys and extracting keybits in the process.
"""
import math
import random
from typing import Dict, List, Tuple
from core.mmm import core_input_masks
from core.correlation import corr
from core.toolbox import P8, GF2_8, signum, subbyte
from collections import Counter

Vector = Tuple[int, int, int, int]
Key = Tuple[int, int, int]
Inequalities = Tuple[int, int, int, int]

INEQtoKEY: Dict[Inequalities, Key] = {
    (1, 1, 1, -1): (0, 0, 0),
    (1, 1, -1, 1): (0, 0, 1),
    (1, -1, 1, 1): (0, 1, 0),
    (-1, 1, 1, 1): (0, 1, 1),
    (1, -1, -1, -1): (1, 0, 0),
    (-1, 1, -1, -1): (1, 0, 1),
    (-1, -1, 1, -1): (1, 1, 0),
    (-1, -1, -1, 1): (1, 1, 1)
}
KEYtoINEQ: Dict[Key, Inequalities] = {v: k for k, v in INEQtoKEY.items()}


def _manhattan(a: Vector, b: Vector) -> float:
    """
    Compute the manhattan distance between vectors `a` and `b`.
    Assumes both vectors have the same size.
    """
    assert len(a) == len(b)
    return sum(abs(x - y) for x, y in zip(a, b))


def manhattan(vectors: List[Vector]) -> Dict[Key, float]:
    """
    Likelihood estimator for key, based on Manhattan distance metric.
    """

    # Compute coordinate
    vector = []
    c = Counter(vectors)
    for v in [(0, 0, 0), (0, 0, 1), (0, 1, 0), (0, 1, 1)]:
        x, y = c.get((*v, 0), 0), c.get((*v, 1), 0)
        try:
            vector.append((x - y) / (x + y))
        except ZeroDivisionError:
            vector.append(0)

    # Compute manhattan distance metric.
    manhattan_metric = {key: _manhattan(ineq, vector)
                        for ineq, key in INEQtoKEY.items()}

    return manhattan_metric


def choose(n, k):
    "Returns n choose k"
    return int(math.perm(n, k) / math.factorial(k))


def CDF_binomial(p, n, k) -> float:
    """
    Compute CDF of binomial distribution.
    :param p: probability of success.
    :param n: number of samples.
    :param k: number of successes.
    """
    return sum(
        choose(n, i) * (p ** i) * (1 - p) ** (n - i)
        for i in range(k + 1)
    )


def gap(vectors: List[Vector]) -> Dict[Inequalities, float]:
    """
    Likelihood estimator for inequalities, based on gap-size.
    """
    gap_metric = {}

    # Compute gap probabilities
    c = Counter(vectors)
    for v in [(0, 0, 0), (0, 0, 1), (0, 1, 0), (0, 1, 1)]:
        k, n_k = c.get((*v, 0), 0), c.get((*v, 1), 0)
        n = k + n_k

        # Case: >
        prob_h0 = 1 - CDF_binomial(38/64, n, k - 1)
        prob_h1 = CDF_binomial(42/64, n, k)
        gap_metric[v] = prob_h1 / (prob_h0 + prob_h1)  # Normalized

        # Case: <
        nv = tuple(i ^ 1 for i in v)
        prob_h0 = 1 - CDF_binomial(38/64, n, n_k - 1)
        prob_h1 = CDF_binomial(42/64, n, n_k)
        gap_metric[nv] = prob_h1 / (prob_h0 + prob_h1)  # Normalized

    # Normalize
    prob_sum = sum(gap_metric.values())
    gap_metric = {k: v / prob_sum for k, v in gap_metric.items()}

    return gap_metric


def guess_3bit_key(vectors: List[Tuple[int, int, int, int]]) -> Tuple[int, int, int]:
    """
    Guess three key bits based on the observed `vectors`.
    """
    metric = manhattan(vectors)
    print("==============")
    for k,v in metric.items():
        print(f"{k}: {v:.2f}")
    metric = gap(vectors)
    print("--------------")
    for k,v in metric.items():
        print(f"{k}: {v:.2f}")
    m = max(metric.items(), key=lambda x: x[1])
    return m[0]


def linear_system_solving_esque(ss: int, exps):
    """
    :param ss: sample size
    :param exps: nr experiments
    """
    opm = 0x01
    ipms = list(next(core_input_masks(opm)))
    init_signs = [int(.5 - .5 * signum(corr(ipm, opm))) for ipm in ipms]

    off_by = [0, 0, 0, 0]
    for _ in range(exps):
        # Select key
        key = random.choice(GF2_8)
        key_parities = tuple(P8(key & ipm) for ipm in ipms)

        # Create data
        pts = [random.choice(GF2_8) for _ in range(ss)]
        cts = [subbyte(p ^ key) for p in pts]

        # Compute equations
        res = []
        for pt, ct in zip(pts, cts):
            ipv = tuple(P8(pt & ipm) for ipm in ipms)
            op = P8(ct & opm)

            # Account for non-positive coefficients
            ipv = tuple(i ^ j for i, j in zip(ipv, init_signs))

            # Enforce ipv starts with 0
            if ipv[0]:
                ipv = tuple(i ^ 1 for i in ipv)
                op = op ^ 1
            res.append((*ipv, op))

        # Indicate expected key parities
        exp_key_parities = guess_3bit_key(res)
        # print("actual:", invLUT[key_parities])
        # print("guessed parities:", exp_key_parities)
        # print("parities:", key_parities)

        # Verify expectations
        distance = sum(i ^ j for i, j in zip(key_parities, exp_key_parities))
        if distance >= 1:
            print(f"{exp_key_parities} should be {key_parities}")
        off_by[distance] += 1

    print(f"SETTINGS:\n  sample size = {ss}\n  #experiments = {exps}")
    print("RESULTS:")
    for i, x in enumerate(off_by):
        print(f"  off by {i}: {x} / {exps} = {x / exps * 100:.2f}%")


if __name__ == "__main__":
    linear_system_solving_esque(16, 128)
