from collections import Counter
from typing import List


GF2_8 = range(0, 256)
REDUCTOR = 283


def cylesh8_1(x: int, steps: int) -> int:
    return (x << steps ^ x >> 8 - steps) & 0xFF


def mulp(x, y) -> int:
    # mulp
    res = 0
    while x:
        if x & 1:
            res ^= y
        y <<= 1
        x >>= 1

    #mod
    val = 283 << 8
    for i in range(8):
        val >>= 1
        if (res ^ val) < res:
            res = res ^ val
    return res

def get_inverse(x: int) -> int:
    if x == 0:
        return 0
    for j in GF2_8:
        res = mulp(x, j)
        if res == 1:
            return j
    raise ValueError(x)


def transform(x: int) -> int:
    return x ^ cylesh8_1(x, 1) ^ cylesh8_1(x, 2) \
             ^ cylesh8_1(x, 3) ^ cylesh8_1(x, 4) ^ 0x63


def get_sbox_LUT():
    mapping = {}
    for i in range(0, 256):
        mapping[i] = transform(get_inverse(i))
    return mapping

def get_parity(x):
    x ^= x >> 4
    x ^= x >> 2
    return (x ^ x >> 1) & 1

def compute_LAT(LUT):
    LAT = [[128]*256 for i in range(256)]
    for ipm in GF2_8:
        for opm in GF2_8:
            LAT[ipm][opm] -= sum(get_parity(ipm & pt ^ opm & LUT[pt]) for pt in GF2_8)
    return LAT

def print_LAT_table(table, indent=4):
    print(
        f'    {" ".join(f"{x:X}".rjust(indent) for x in range(len(table[0])))} ')
    print('-'*(5*len(table) + 3))
    for idx, row in enumerate(table):
        print(f'{idx:02X} [{",".join(str(x).rjust(indent) for x in row)}]')


def print_linearized(table, out_idx):
    for idx, row in enumerate(table):
        # if row[out_idx]:
        print(f"{row[out_idx]: 3} * I({' ^ '.join([f'F(pt, {i}]' for i, b in enumerate(reversed(f'{idx:08b}')) if b == '1']) or 0})")


def transpose(table):
    return [[r[i] for r in table] for i in range(256)]

def get_indices(col: List, val):
    """Return all indices in col with abs(val)"""
    return [i for i, elt in enumerate(col) if abs(elt) == val]

def get_bit_count(i):
    return f"{i:b}".count("1")

def print_counters():
    table = transpose(compute_LAT(get_sbox_LUT()))
    for idx, c in enumerate(table):
        if idx == 0x6A:
            x = 5
        count = sum(get_bit_count(i) for i in get_indices(c, 16))
        print(f"{idx:2X}: {count=}")
        # print(f"{idx:2X}:", Counter([abs(e) for e in c]))

# print_LAT_table(transpose(compute_LAT(get_sbox_LUT())))
# print_linearized(compute_LAT(get_sbox_LUT()), 0x74)
# # print(f'{mulp(0x57,0x83):X}')
# print_counters()

# x=[]
# -16 * I(F(pt, 0] ^ F(pt, 1] ^ F(pt, 4] ^ F(pt, 5] ^ F(pt, 6] ^ F(pt, 7])
# -16 * I(F(pt, 0] ^ F(pt, 2] ^ F(pt, 5] ^ F(pt, 6])
# -16 * I(F(pt, 0] ^ F(pt, 5] ^ F(pt, 6] ^ F(pt, 7])
# -16 * I(F(pt, 1] ^ F(pt, 2] ^ F(pt, 4] ^ F(pt, 7])
# -16 * I(F(pt, 2] ^ F(pt, 7])

# print(get_bit_count(0))

def I(x):
    return 1-(x << 1)

def F(x, i):
    return (x >> i) & 0x1

def find_best_orientation():
    sbox = get_sbox_LUT()
    OPM = 0x74
    for a in range(32):
        mapping = {}
        countmap = {}
        i1, i2, i3, i4, i5 = I(F(a, 0)), I(F(a, 1)), I(F(a, 2)), I(F(a, 3)), I(F(a, 5))
        for i in range(256):
            out_parity = get_parity(sbox[i] & OPM)

            val = (i1* I(F(i, 1) ^ F(i, 2)) \
                + i2 * I(F(i, 1) ^ F(i, 2) ^ F(i, 4) ^ F(i, 7))\
                + i3 * I(F(i, 1) ^ F(i, 4))\
                + i4 * I(F(i, 1) ^ F(i, 7))\
                + i5 * I(F(i, 2) ^ F(i, 4)))

            mapping.setdefault(val, 0)
            mapping[val] += out_parity
            countmap.setdefault(val, 0)
            countmap[val] += 1

        output = {}
        for key, val in countmap.items():
            output[key] = (mapping[key] / val > 0.5)
        output["total"] = sum((1- output[key]) * mapping[key]  + output[key] * (countmap[key] - mapping[key]) for key in countmap)

        print(a, output, mapping, countmap)
        # print(count, mapping, countmap)


def compute_correlation():
    sbox = get_sbox_LUT()
    OPM = 0x74
    count = 0
    for pt in range(256):
        a1 = I(F(pt, 1) ^ F(pt, 2))
        a2 = I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 4) ^ F(pt, 7))
        a3 = I(F(pt, 1) ^ F(pt, 4))
        a4 = I(F(pt, 1) ^ F(pt, 7))
        a5 = I(F(pt, 2) ^ F(pt, 4))
        in_par = a1 + a2 + a3 + a4 + 2 * a5
        print(f'{a1=: 2}, {a2=: 2}, {a3=: 2}, {a4=: 2}, {a5=: 2}, {in_par=:2}')
        out_par = get_parity(sbox[pt] & OPM)
        count += (in_par >= 0) ^ out_par
    return 2 * (count / 256) - 1


def compute_correlation_v2():
    sbox = get_sbox_LUT()
    OPM = 0x74
    count = 0
    for pt in range(256):
        x = [
             I(F(pt, 0) ^ F(pt, 2)),
             I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 2)),
            -I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 3)),
             I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 3) ^ F(pt, 4)),
            -I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 3) ^ F(pt, 5)),
            -I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 5)),
             I(F(pt, 0) ^ F(pt, 2) ^ F(pt, 4) ^ F(pt, 5)),
            -I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 2) ^ F(pt, 4) ^ F(pt, 5)),
             I(F(pt, 1) ^ F(pt, 2)),
            -I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 6) ^ F(pt, 7)),
             I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 4) ^ F(pt, 6)),
             I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 4) ^ F(pt, 7)),
            -I(F(pt, 1) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 6)),
             I(F(pt, 1) ^ F(pt, 4)),
             I(F(pt, 1) ^ F(pt, 5) ^ F(pt, 6)),
             I(F(pt, 1) ^ F(pt, 7)),
            -I(F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 6)),
             I(F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 6) ^ F(pt, 7)),
             I(F(pt, 2) ^ F(pt, 4)),
            -I(F(pt, 2) ^ F(pt, 6) ^ F(pt, 7)),
             I(F(pt, 4) ^ F(pt, 5) ^ F(pt, 6) ^ F(pt, 7)),
        ]

        in_par = sum(x)
        out_par = get_parity(sbox[pt] & OPM)
        count += (in_par >= 0) ^ out_par
    return 2 * (count / 256) - 1

# print(compute_correlation_v2())
sbox = get_sbox_LUT()
for i in range(256):
    print(f"0x{sbox[i]:02X}")