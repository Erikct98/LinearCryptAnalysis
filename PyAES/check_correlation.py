from toolbox import P8, sbox, GF2_8


def check_correlation(ipm, opm):
    count = 0
    for pt in GF2_8:
        in_parity = P8(pt & ipm)
        output_parity = P8(sbox[pt] & opm)
        count += in_parity ^ output_parity
    return 128 - count

if __name__ == "__main__":
    print(check_correlation(0x1, 0x3))
