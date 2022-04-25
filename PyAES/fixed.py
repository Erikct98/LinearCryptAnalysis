"""
Library used in investigating what happens when you (partially) fix
input to the AES.
"""
from core.toolbox import P32, subbyte, cylesh32
from core.mixcolumn import mixcolumn
from random import randint
from typing import Callable
from matplotlib import pyplot as plt


def _mix_and_subbyte(pt: int) -> int:
    """
    MixColumn 32-bit word `pt` and then apply the sbox to each byte.
    """
    mt = mixcolumn(pt)
    ct = cylesh32(mt & 0xFFFFFF00 ^ subbyte(mt & 0xFF), 8)
    ct = cylesh32(ct & 0xFFFFFF00 ^ subbyte(ct & 0xFF), 8)
    ct = cylesh32(ct & 0xFFFFFF00 ^ subbyte(ct & 0xFF), 8)
    ct = cylesh32(ct & 0xFFFFFF00 ^ subbyte(ct & 0xFF), 8)
    return ct


def fix_one_byte_mix_subbyte(ipm: int, opm: int, fixed: int):
    """
    Fix first byte in 32-bit word. Apply mixcolumn and subbyte.
    Find correlation between input mask and output mask.
    """
    SAMPLE_SIZE = 0x1000000
    assert 0 <= fixed <= 255

    fixed = fixed << 24

    count = 0
    for pt in range(SAMPLE_SIZE):
        pt = pt ^ fixed
        ct = _mix_and_subbyte(pt)

        count += P32(pt & ipm ^ ct & opm)

    return count, SAMPLE_SIZE


def fix_one_byte_view_possible_outputs(fixed: int):
    """
    Fix first byte in 32-bit word. Apply mixcolumn and subbyte.
    Investigate the (partial) output.
    """
    SAMPLE_SIZE = 0x100000
    assert 0 <= fixed <= 255

    fixed = fixed << 24

    def func():
        """
        Plot for first byte after MC + SB
        """
        pt = randint(0, 0xFFFFFF)
        pt = pt ^ fixed
        ct = _mix_and_subbyte(pt)
        return (ct >> 24) & 0xFF

    plot_function_hist(SAMPLE_SIZE, func)

    def random():
        return randint(0, 0xFF)

    plot_function_hist(SAMPLE_SIZE, random)

def plot_function_hist(sample_size: int, func: Callable) -> None:
    """
    Plot histogram by sampling `sample_size` datapoints from function `func`
    """
    mapping = {}
    for _ in range(sample_size):
        res = func()
        mapping.setdefault(res, 0)
        mapping[res] += 1

    fig, ax = plt.subplots(figsize = (10, 7))
    res = mapping.values()
    ax.hist(mapping.values(), bins=16)
    plt.show()


if __name__ == "__main__":
    pass

