from typing import List
# Find a way to determine which trace is best

def get_SIMON_SBOX(gate_size: int):
    max_value = (1 << gate_size) - 1
    def _sbox(val: int):
        """
        SBOX = round of SIMON except key addition.
        :param val: SIMON LHS input to perform entire Sbox on.
        """
        assert 0 <= val <= max_value, f"0 <= {val} <= {max_value}"
        def lcs(val, nr_bits):
            """
            Cyclically shift `val` to the left by `nr_bits`.
            """
            shifted = val << nr_bits
            LHS, RHS = shifted & max_value, shifted >> gate_size
            return LHS ^ RHS
        return (lcs(val, 1) & lcs(val, 8)) ^ lcs(val, 2)
    return _sbox

def ip(word_one, word_two, word_size):
    xor = word_one ^ word_two
    res = 0
    for i in range(word_size):
        res ^= xor & 1
        xor >>= 1
    return res

def get_correlation_matrix(word_size: int) -> List[List[float]]:
    """
    :param word_size: number of bits of the entire word (both halves)
    """
    gate_size = word_size >> 1
    nr_inputs = 1 << gate_size
    
    SBOX = get_SIMON_SBOX(gate_size)    
    results = [None] * nr_inputs
    for x in range(nr_inputs):
        results[x] = SBOX(x)
    
    # print(results)
    
    corr_table = [[0] * nr_inputs for _ in range(nr_inputs)]
    for ipm in range(nr_inputs):
        for opm in range(nr_inputs):
            for inp, out in enumerate(results):
                corr_table[ipm][opm] += ip(ipm ^ inp, opm ^ out, gate_size)
                
    return corr_table

# C = get_correlation_matrix(32)
# print(C)

