"""
This library assists in finding non-linear trails through the
AES sbox that hold with high probability.

E.g.
Let sbox(pt) = ct.
The trail (pt3 | pt7) ^ (pt5 | pt1) == ct7 ^ ct3 ^ ct1 holds with correlation 0.2188.
Observe here the use of non-linear OR function (|)
"""

from core.toolbox import GF2_8
from core.LAT import LAT

def find_4_term_non_linear(opm):
    """
    Seeks 4 linear terms that can be combined into one single, non-linear term
    that can predict the output mask `opm` with high probability.
    :returns: list of tuples, sorted from most interesting to least interesting.
    """
    # Find input masks with correlation 16/128 = 1/8 with output mask.
    sixteens = [ipm for ipm in GF2_8 if abs(LAT[ipm][opm]) == 16]

    # Iterate through all 4-term combinations.
    results = []
    for s in sixteens:
        for i in range(8):
            for j in range(i + 1, 8):
                # Compute masks neighbouring to `s`
                b1, b2 = 1 << i, 1 << j
                mask = b1 ^ b2
                zero = s & ~mask
                one = zero ^ b1
                two = zero ^ b2
                three = zero ^ mask

                # Compute correlation.
                correlations = (LAT[zero][opm], LAT[one][opm], LAT[two][opm], LAT[three][opm])
                min_corr_mag = min([abs(x) for x in correlations])
                masks = (f'0x{zero:X}', f'0x{one:X}', f'0x{two:X}', f'0x{three:X}')
                results.append((min_corr_mag, masks, correlations))

    # Sort results to get best first.
    results.sort(reverse=True)

    return results

if __name__ == "__main__":
    terms = find_4_term_non_linear(0x7A)
    for term in terms:
        print(term)
