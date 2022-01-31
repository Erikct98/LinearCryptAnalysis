from linearization_finder import GF2_8, P, I, J, sbox

GF2_8 = range(256)

def check_correlation(ipm, opm):
    count = 0
    for pt in GF2_8:
        in_parity = P(pt & ipm)
        output_parity = P(sbox[pt] & opm)
        count += in_parity ^ output_parity
    return 128 - count

if __name__ == "__main__":
    print(check_correlation(0x1, 0x3))
