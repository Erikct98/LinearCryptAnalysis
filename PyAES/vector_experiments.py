import itertools
import random
from counting import Counter
from typing import Dict, Tuple
from core.toolbox import GF2_8, subbyte, P8, I, hex_, BITS, xorsum
from core.mmm import core_input_masks


def all_plaintext_generator(nr_sboxes):
    """
    Generates all plaintexts for `nr_sboxes` parallel sboxes
    """
    yield from itertools.product(GF2_8, repeat=nr_sboxes)


def random_plaintext_generator(nr_sboxes, samplesize):
    """
    Generates `samplesize` random plaintexts for `nr_sboxes` parallel sboxes
    """
    for _ in range(samplesize):
        yield tuple(random.choice(GF2_8) for _ in range(nr_sboxes))


def vector_frequencies(opms=[0x01], keys=[0x00], ptgen=None, encrypt=None):
    """
    Compute the frequencies of a input/output vector occurring when working
    with #`opms` parallel sboxes with initial `key` xor.
    Assumes #`opms` == #`keys`.
    """

    # Checks
    assert len(opms) == len(keys)
    nr_sboxes = len(opms)

    # Choose all plaintext generator if not defined
    if ptgen is None:
        ptgen = all_plaintext_generator(nr_sboxes=nr_sboxes)

    # Choose encryption if not specified
    if encrypt is None:
        encrypt = subbyte

    # Iterate
    veclen = 3 * nr_sboxes + 1
    vector_cnts = {k: 0 for k in itertools.product(BITS, repeat=veclen)}
    mask_groups = [tuple(next(core_input_masks(opm))) for opm in opms]
    for pts in ptgen:
        ipbs = [x
                for masks, pt in zip(mask_groups, pts)
                for x in [P8(pt & m) for m in masks]]
        opbs = [P8(encrypt(pt ^ k) & m) for pt, k, m in zip(pts, keys, opms)]
        opp  = xorsum(opbs)
        vector_cnts[(*ipbs, opp)] += 1

    return vector_cnts


def print_vectors(frequencies: Dict[Tuple, int]):
    for k, v in frequencies.items():
        print(k, v)
    # print(dict(sorted(Counter(frequencies.values()).items())))


def print_table(frequencies: Dict[int, Dict[Tuple, int]]):
    keys = list(frequencies.keys())
    bitmasks = sorted(frequencies[keys[0]].keys(), key=lambda x: (x[3], x[0], x[1], x[2]))
    offset = 3 * len(bitmasks[0])

    # Print toprow
    print(" " * offset, *[f"{hex_(k):>4}" for k in keys])

    for bm in bitmasks:
        print(bm, *[f"{frequencies[k][bm]:>4}" for k in keys])


def print_diff_table(frequencies: Dict[int, Dict[Tuple, int]]):
    keys = list(frequencies.keys())
    bitmasks = list(frequencies[keys[0]].keys())
    offset = 3 * (len(bitmasks[0]) - 1)

    # Print toprow
    print(" " * offset, *[f"{hex_(k):>5}" for k in keys])

    subset = [bm for bm in bitmasks if bm[-1] == 0]

    for bm in subset:
        contra = (*bm[:-1], 1)
        print((*bm[:-1],), *[f"{(frequencies[k][bm] - frequencies[k][contra]):>5}" for k in keys])


if __name__ == "__main__":
    nr_sboxes = 4
    ss = (16 ** nr_sboxes) * 4

    shuffler = list(GF2_8)
    random.shuffle(shuffler)
    def randomizer(x):
        return shuffler[x]

    ptgen = random_plaintext_generator(nr_sboxes, ss)
    freqs = vector_frequencies(opms=[0x2D, 0x5F, 0x9B, 0x78], keys=[0x3C, 0x01, 0x3F, 0xD2], ptgen=ptgen)
    freqs = {k: v for k,v in sorted(freqs.items(), key=lambda x: x[1])}

    hist = Counter(freqs.values())
    hist = {k:v for k,v in sorted(hist.items(), key=lambda x: x[0])}
    print(hist)
    pass
