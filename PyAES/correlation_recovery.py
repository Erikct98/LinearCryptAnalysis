from core.toolbox import GF2_8, I, P8, bit_count, export_table, signum
import math

data_5mm = {
    0: 68460458,
    1: 67852894,
    2: 67858326,
    3: 67272354,
    4: 67856670,
    5: 67262370,
    6: 67268522,
    7: 66696646,
    8: 67991542,
    9: 67400930,
    10: 67401482,
    11: 66835598,
    12: 67411514,
    13: 66823622,
    14: 66813166,
    15: 66223666,
    16: 68005062,
    17: 67379194,
    18: 67389522,
    19: 66797342,
    20: 67401482,
    21: 66820638,
    22: 66824518,
    23: 66231250,
    24: 67543242,
    25: 66930374,
    26: 66942878,
    27: 66354546,
    28: 66957982,
    29: 66362250,
    30: 66331938,
    31: 65781670,
    32: 68399830,
    33: 67941410,
    34: 67934698,
    35: 67466974,
    36: 67940962,
    37: 67473374,
    38: 67465430,
    39: 66995898,
    40: 67807370,
    41: 67331998,
    42: 67333238,
    43: 66857714,
    44: 67326790,
    45: 66865082,
    46: 66876818,
    47: 66424910,
    48: 67785914,
    49: 67362182,
    50: 67353134,
    51: 66887522,
    52: 67336566,
    53: 66867810,
    54: 66865722,
    55: 66417582,
    56: 67193526,
    57: 66760890,
    58: 66746594,
    59: 66293518,
    60: 66727906,
    61: 66290422,
    62: 66319454,
    63: 65832410
}

data_3mm = {
0: 268500416,
1: 268364592,
2: 268562376,
3: 268454840,
4: 268426984,
5: 268300968,
6: 268499760,
7: 268373712,
8: 268370496,
9: 268506320,
10: 268308536,
11: 268416072,
12: 268443928,
13: 268569944,
14: 268371152,
15: 268497200
}

GF2_3 = range(8)
GF2_4 = range(16)
GF2_5 = range(32)
GF2_6 = range(64)


def correlation():
    maxval = 0

    table = [list() for _ in range(64)]
    for bma in GF2_6:
        for bmb in GF2_6:
            total = 0
            for idx, cnt in data_5mm.items():
                x = I(P8(bma & idx) ^ P8(bmb & idx))
                total += x * cnt
            corr = total / (2 ** 32)
            assert corr >= -1 and corr <= 1

            if abs(corr) > maxval and abs(corr) < 1:
                maxval = corr

            if abs(corr) >= 2**(-8):
                table[bma].append(math.log2(corr))
            else:
                table[bma].append("....")
    # print(maxval)
    export_table(table)


def majority_5mm():
    total = 0
    for i in GF2_5:
        maj = bit_count(i) >= 3
        total += data_5mm[i ^ (maj << 5)]

    total = total / 2**31 - 1
    print(total, math.log2(abs(total)))

def majority_3mm():
    total = 0
    for i in GF2_3:
        maj = bit_count(i) >= 2
        total += data_3mm[i ^ (maj << 3)]

    total = total / 2**31 - 1
    print(total, math.log2(abs(total)))


def bitcount_5mm():
    x = list(data_5mm.items())
    x = sorted(x, key=lambda x: x[1])
    y = map(lambda x: (bit_count(x[0] % 32), x[1]), x)
    for bc, cnt in y:
        print(bc, cnt)


def bitcount_3mm():
    x = list(data_3mm.items())
    x = sorted(x, key=lambda x: x[1])
    y = map(lambda x: (bit_count(x[0] % 8), x[1]), x)
    for bc, cnt in y:
        print(bc, cnt)


def max_corr_3mm():
    total = 0
    choice = []
    for i in GF2_3:
        val = data_3mm[i] - data_3mm[i + 8]
        total += abs(val)
        choice.append(signum(val))
    print(math.log2(total/2**31), choice)

def custom_corr_3mm():
    total = 0
    for i in GF2_3:
        choice = (bit_count(i) >= 2) ^ (i >> 2) ^ (i & 1) ^ 1
        total += data_3mm[i ^ (choice << 3)]
    print(math.log2(abs(total/2**31 - 1)), choice, total)


if __name__ == "__main__":
    majority_3mm()
    bitcount_3mm()
    max_corr_3mm()
    custom_corr_3mm()
