from core.toolbox import GF2_8, P8, I, F, subbyte


def find_optimal_signs():
    """
    EXPERIMENT
    Investigates which combination of signs for each of the terms gives
    the greatest correlation.
    """
    OPM = 0x74
    for a in range(32):
        mapping = {}
        countmap = {}
        i1, i2, i3, i4, i5 = I(F(a, 0)), I(F(a, 1)), I(F(a, 2)), I(F(a, 3)), I(F(a, 5))

        for pt in GF2_8:
            val = (i1 * I(F(pt, 1) ^ F(pt, 2))
                   + i2 * I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 4) ^ F(pt, 7))
                   + i3 * I(F(pt, 1) ^ F(pt, 4))
                   + i4 * I(F(pt, 1) ^ F(pt, 7))
                   + i5 * I(F(pt, 2) ^ F(pt, 4)))

            out_parity = P8(subbyte(pt) & OPM)

            mapping.setdefault(val, 0)
            mapping[val] += out_parity
            countmap.setdefault(val, 0)
            countmap[val] += 1

        output = {}
        for key, val in countmap.items():
            output[key] = (mapping[key] / val > 0.5)
        output["total"] = sum((1 - output[key]) * mapping[key] + output[key]
                              * (countmap[key] - mapping[key]) for key in countmap)

        print(a, output, mapping, countmap)


def compute_correlation():
    """
    EXPERIMENT
    Compute the correlation of a sum of a couple linear terms.
    :returns: tuple with correlation and 256*prob.
    """
    OPM = 0x74
    count = 256
    for pt in GF2_8:
        a1 = I(F(pt, 1) ^ F(pt, 2))
        a2 = I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 4) ^ F(pt, 7))
        a3 = I(F(pt, 1) ^ F(pt, 4))
        a4 = I(F(pt, 1) ^ F(pt, 7))
        a5 = I(F(pt, 2) ^ F(pt, 4))
        in_par = a1 + a2 + a3 + a4 + a5
        out_par = P8(subbyte(pt) & OPM)
        count -= (in_par >= 0) ^ out_par
    return (2 * (count / 256) - 1, count)


def compute_correlation_v2():
    """
    EXPERIMENT
    Compute the correlation of a sum of couple linear terms.
    :returns: tuple with correlation and 256*prob.
    """
    OPM = 0x74
    count = 256
    for pt in GF2_8:
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

        in_par = sum(x) >= 0
        out_par = P8(subbyte(pt) & OPM)
        count -= in_par ^ out_par
    return (2 * (count / 256) - 1, count)


if __name__ == "__main__":
    pass
