from toolbox import GF2_8
from LAT import LAT

def find_interesting_and_combination(opm):
    # find 16s
    sixteens = [ipm for ipm in GF2_8 if abs(LAT[ipm][opm]) == 16]
    for s in sixteens:
        for i in range(8):
            for j in range(i + 1, 8):
                first_bit = 1 << i
                second_bit = 1 << j
                mask = first_bit ^ second_bit
                zero = s & ~mask
                one = zero ^ first_bit
                two = zero ^ second_bit
                three = zero ^ mask
                print(s, i, j, (LAT[zero][opm], LAT[one][opm], LAT[two][opm], LAT[three][opm]) )
                if s == 168 and i == 1 and j == 5:
                    print("hello")

find_interesting_and_combination(0x7A)

# 168 1 5 (12, -12, -16, -12)
# 171 0 1 (-16, 8, -12, -16)
# 171 0 7 (-14, -10, -12, -16)