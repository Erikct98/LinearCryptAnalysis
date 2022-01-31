from linear_function import LinearFunction
from LAT import LAT
from toolbox import GF2_8, J, P8, I
from correlation import func_corr, keyed_func_corr


def keyed_linearization_for_ipm(ipm: int, ipkey: int = 0) -> LinearFunction:
    """
    Returns function that computes parity of input mask `ipm` from output bits
    with probability = 1.
    Through: xor with `key` and SubBytes step.
    """
    coeffs = [I(P8(ipkey & opm)) * LAT[ipm][opm] for opm in GF2_8]
    offset = (1 - (sum(coeffs) >> 7)) >> 1

    def _func(ct: int) -> int:
        linear = sum(coeffs[opm] * P8(ct & opm) for opm in GF2_8)
        return offset + (linear >> 7)
    return _func


def keyed_linearization_for_opm(opm: int, opkey: int = 0) -> LinearFunction:
    """
    Returns function that computes parity of output mask `opm` from input bits
    with probability = 1.
    Through: xor with `key` and SubBytes step.
    """
    coeffs = [I(P8(opkey & ipm)) * LAT[ipm][opm] for ipm in GF2_8]
    offset = (1 - (sum(coeffs) >> 7)) >> 1

    def _func(pt: int) -> int:
        linear = sum(coeffs[ipm] * P8(pt & ipm) for ipm in GF2_8)
        return offset + (linear >> 7)
    return _func


def test_linearization_for_ipm():
    ipm = 0x5A
    def ipf(pt): return P8(pt & ipm)
    opf = keyed_linearization_for_ipm(ipm)
    assert func_corr(ipf, opf) == 128


def test_linearization_for_ipm_with_key():
    ipm = 0x5A
    ipkey, opkey = 0x00, 0x9A
    def ipf(pt): return P8(pt & ipm)
    opf = keyed_linearization_for_ipm(ipm, opkey)
    assert keyed_func_corr(ipf, opf, ipkey, opkey) == 128


def test_linearization_for_opm():
    opm = 0x5A
    ipf = keyed_linearization_for_opm(opm)
    def opf(ct): return P8(ct & opm)
    assert func_corr(ipf, opf) == 128


def test_linearization_for_opm_with_key():
    opm = 0x5A
    ipkey, opkey = 0x9A, 0x00
    ipf = keyed_linearization_for_opm(opm, ipkey)
    def opf(ct): return P8(ct & opm)
    assert keyed_func_corr(ipf, opf, ipkey, opkey) == 128


if __name__ == "__main__":
    pass
