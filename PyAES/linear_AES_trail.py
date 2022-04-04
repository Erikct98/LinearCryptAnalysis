"""
Aids in the creation of a linear trail through AES.
"""

from typing import List
from mixcolumn_trail_search import find_input_mask, find_output_mask
from toolbox import join32, split32
from correlation import ipm16, opm16


def print_mask(mask: List[int]) -> None:
    """
    Print mask to console.
    """
    chunks = [split32(m) for m in mask]
    for chunk in zip(*chunks):
        print(" ".join(f"0x{c:02X}" for c in chunk))


def forward_trail_one_round(ipm: List[int]):
    """
    Trail over one round of AES.
    """
    print("input mask")
    print_mask(ipm)

    # Trail over sbox
    chunks = [split32(m) for m in ipm]
    chunks = [[opm16(x) for x in c] for c in chunks]
    chunks = [join32(*c) for c in chunks]
    print("mask after sbox")
    print_mask(chunks)

    # Trail over shiftrows
    chunks = [split32(m) for m in chunks]
    chunks = [[c[o] for o, c in enumerate(chunks[i:] + chunks[:i])] for i in range(4)]
    chunks = [join32(*c) for c in chunks]
    print("mask after shiftrows")
    print_mask(chunks)

    # Trail over mixcolumns
    chunks = [find_output_mask(m) for m in chunks]
    print("mask after mixcolumns")
    print_mask(chunks)

    return chunks


def backward_trail_one_round(opm: List[int]):
    """
    Trail over one round of AES, in reverse
    """
    print("input mask")
    print_mask(opm)

    # Trail over mixcolumns
    chunks = [find_input_mask(m) for m in opm]
    print("mask after mixcolumns")
    print_mask(chunks)

    # Trail over shiftrows
    chunks = [split32(m) for m in chunks]
    chunks = [[c[o] for o, c in enumerate(list(reversed(chunks[:i+1])) + list(reversed(chunks[i+1:])))] for i in range(4)]
    chunks = [join32(*c) for c in chunks]
    print("mask after shiftrows")
    print_mask(chunks)

    # Trail over sbox
    chunks = [split32(m) for m in chunks]
    chunks = [[ipm16(x) for x in c] for c in chunks]
    chunks = [join32(*c) for c in chunks]
    print("mask after sbox")
    print_mask(chunks)

    return chunks


def make_trail(rounds, ipm, after_round):

    print("BACKWARD TRAILING")
    mask = ipm
    for _ in range(after_round):
        mask = backward_trail_one_round(mask)

    print("FORWARD TRAILING")
    mask = ipm
    for _ in range(after_round, rounds):
        mask = forward_trail_one_round(mask)



if __name__ == "__main__":
    make_trail(4, [0x2D000000, 0x00000000, 0x00000000, 0x00000000], 1)
    # forward_trail_one_round([0x2D000000, 0x00000000, 0x00000000, 0x00000000])


    pass