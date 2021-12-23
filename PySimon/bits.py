from typing import List, Tuple
from contextlib import contextmanager

# def int2bin(x: int) -> str:
#     return f'{x:06b}'

# def bin2int(x: str) -> int:
#     return int(x, 2)

# def masker(nr_bits):
#     """iterator of all masks"""
#     for i in range(1 << nr_bits):
#         yield i


from Sbox import SIMONbox

def make_SIMON_Sbox():
    s = SIMONbox(8)
    def _sbox(x: int):
        return (s.cls(x, 1) & s.cls(x, 8)) ^ s.cls(x, 2)
    return _sbox

def make_DES_Sbox(box_table: List[List[int]]):
    def _sbox(x: int):
        """Computes the output of a DES S-box according to the given table
        :param B: input block
        :returns: output block
        """   
        i = ((x & (1 << 5)) >> 4) ^ (x & 1)
        j = (x % (1 << 5)) >> 1
        return box_table[i][j]
    return _sbox

def make_example_Sbox(box_table: List[int]):
    def _sbox(x: int):
        return box_table[x]
    return _sbox

def make_example_Sbox2(box_table: List[int], K1: int, K2: int):
    def _sbox(x: int):
        try:
            return box_table[x ^ K1] ^ K2
        except IndexError as e:
            print(f"index {x^K2}, {x}, {K1}, {K2}")
            raise e
            
    return _sbox

def make_test_Sbox(K1: int, K2: int):
    assert 0 <= K1 <= 1 and 0 <= K2 <= 1
    def _sbox(x: int):
        assert 0 <= x <= 15
        return x ^(K1 << 3 )^( K2 << 2)^(K1 << 1)^ K2
    return _sbox

def make_test_Sbox2(K1: int, K2: int):
    assert 0 <= K1 <= 1 and 0 <= K2 <= 1
    def _sbox(x: int):
        assert 0 <= x <= 3
        return x ^(K1 << 1)^ K2
    return _sbox

def make_fork_Sbox(in_size: int):
    def _sbox(x: int):
        assert 0 <= x < 1 << in_size
        return x << in_size ^x
    return _sbox

def make_AND_Sbox(in_size: int):
    def _sbox(x: int):
        assert 0 <= x < 1 << (2 * in_size)
        left, right = x >> in_size, x & ((1 << in_size) - 1)
        return left & right
    return _sbox

def make_aes_esque_sbox():
    def ls(x, steps):
        return ((x << steps) ^ (x >> (4 - steps)) & 0xF)
    def _sbox(x: int):
        x2, x3, x4 = x & 0xF, (x >> 4) & 0xF, (x >> 8) & 0xF
        y1 = x2 ^ ls(x3, 1) ^ ls(x4, 2)
        return y1
    
        # x1, x2, x3, x4 = x & 0xF, (x >> 4) & 0xF, (x >> 8) & 0xF, (x >> 12) & 0xF    
        # y2 = x3 ^ ls(x4, 1) ^ ls(x1, 2)
        # y3 = x4 ^ ls(x1, 1) ^ ls(x2, 2)
        # y4 = x1 ^ ls(x2, 1) ^ ls(x3, 2)
        # return (y4 << 12) ^ (y3 << 8) ^ (y2 << 4) ^ y1
    return _sbox

def parity(x: int):
    nr_it = len(f'{x:b}')
    parity = 0
    for i in range(nr_it):
        parity ^= x & 1
        x >>= 1
    return parity

def compute_LAP(SBOX, input_len, output_len):
    """Compute nr of zero-parities"""
    input_mask = range(1 << input_len)
    output_mask = range(1 << output_len)
    
    outputs = {}
    for i in range(1 << input_len):
        outputs[i] = SBOX(i)
        
    half = 1 << (input_len - 1)
    LAP = [[-half 
            for _ in range(1 << output_len)] 
            for __ in range(1 << input_len)]
    for ip, op in outputs.items():
        for ipm in input_mask:
            input_parity = parity(ipm & ip) ^ 1
            for opm in output_mask:
                # print((ipm, ip), (opm, op), (ipm & ip) ^ (opm & op))
                LAP[ipm][opm] += parity(opm & op) ^ input_parity

    return LAP

DESBOX_strings = {
1:
"""14 4 13 1 2 15 11 8 3 10 6 12 5 9 0 7
0 15 7 4 14 2 13 1 10 6 12 11 9 5 3 8
4 1 14 8 13 6 2 11 15 12 9 7 3 10 5 0
15 12 8 2 4 9 1 7 5 11 3 14 10 0 6 13""",
2: 
"""15 1 8 14 6 11 3 4 9 7 2 13 12 0 5 10
3 13 4 7 15 2 8 14 12 0 1 10 6 9 11 5
0 14 7 11 10 4 13 1 5 8 12 6 9 3 2 15
13 8 10 1 3 15 4 2 11 6 7 12 0 5 14 9""",
5 : 
"""2 12 4 1 7 10 11 6 8 5 3 15 13 0 14 9 
14 11 2 12 4 7 13 1 5 0 15 10 3 9 8 6 
4 2 1 11 10 13 7 8 15 9 12 5 6 3 0 14 
11 8 12 7 1 14 2 13 6 15 0 9 10 4 5 3""",
7: 
"""4 11 2 14 15 0 8 13 3 12 9 7 5 10 6 1
13 0 11 7 4 9 1 10 14 3 5 12 2 15 8 6
1 4 11 13 12 3 7 14 10 15 6 8 0 5 9 2
6 11 13 8 1 4 10 7 9 5 0 15 14 2 3 12""",
8:
"""13 2 8 4 6 15 11 1 10 9 3 14 5 0 12 7
1 15 13 8 10 3 7 4 12 5 6 11 0 14 9 2
7 11 4 1 9 12 14 2 0 6 10 13 15 3 5 8
2 1 14 7 4 10 8 13 15 12 9 0 3 5 6 11"""
}


S_table = lambda s: [[int(y) for y in x.strip().split(' ')] for x in DESBOX_strings[s].strip(' ').split('\n')]



def test():
    assert parity(5) == 0
    assert parity(4) == 1
    assert parity(100) == 1
    SBOX = make_DES_Sbox(S_table(5))
    assert SBOX(0) == 2
    assert SBOX(31) == 6
    assert SBOX(32) == 4
    assert SBOX(63) == 3
    
    SBOX = make_AND_Sbox(1)
    assert SBOX(3) == 1
    
def example_sbox_2():
    SBOX = make_example_Sbox2([8, 2, 4, 0, 15, 5, 7, 12, 10, 6, 11, 3, 14, 13, 9, 1], 2, 2)
    table = compute_LAP(SBOX, 4, 4)

    maximum, ridx, cidx = 0, -1, -1
    for idx, row in enumerate(table):
        print(f'{idx:02X} [{",".join(str(x).rjust(4) for x in row)}]')

    for idx, row in enumerate(table[1:], start=1):
        new_max = max(row, key=lambda x: abs(x))
        if abs(new_max) > abs(maximum):
            maximum = new_max
            ridx = idx
            cidx = list(map(lambda x: x == maximum, row)).index(True)
        
    print(f'{ridx:02X}x{cidx:02X}: {maximum}')

def xorsum(word: int, *indices):
    import math
    assert word >= 0
    l = 3
    return sum([
        (word & (1 << (l - i))) >> (l - i) for i in indices
    ]) % 2

def test_exercise_1():
    import random
    k1, k2 = random.randint(1,15), random.randint(1,15)
    assert 0 < k1 < 16 and 0 < k2 < 16
    
    SBOX = make_example_Sbox2([8, 2, 4, 0, 15, 5, 7, 12, 10, 6, 11, 3, 14, 13, 9, 1], k1, k2)
    
    outputs = {}
    for inp in range(16):
        outputs[inp] = SBOX(inp)
        
    approx = {}
    for inp, out in outputs.items():
        approx[(inp, out)] = (xorsum(inp, 0, 3) + xorsum(out, 0, 1, 3)) % 2
    
    key_mask = (xorsum(k1, 0, 3) + xorsum(k2, 0, 1, 3)) % 2
    
    counts = {0: list(approx.values()).count(0), 1: list(approx.values()).count(1)}
    assert counts[key_mask] == 2

def print_LAT_table(table, indent=4):
    for idx, row in enumerate(table):
        print(f'{idx:02X} [{",".join(str(x).rjust(indent) for x in row)}]')

def print_LAT_as_COR_table(table, max_):
    table = [[x / max_ for x in row] for row in table]
    print_LAT_table(table, indent=6)

def compute_max_val(table):
    maximum, ridx, cidx = 0, -1, -1
    for idx, r in enumerate(table[1:], start=1):
        new_max = max(r, key=lambda x: abs(x))
        if abs(new_max) > abs(maximum):
            maximum = new_max
            r_idx = idx
            c_idx = r.index(new_max)
    print(maximum, ridx, cidx)


if __name__ == "__main__":
    test()
    
    # DES Sbox
    # SBOX = make_DES_Sbox(S_table(1))
    # table = compute_LAP(SBOX, 6, 4)
    # print_LAT_table(table)
    # print_LAT_as_COR_table(table, 32)
    
    # import random
    # SBOX = make_DES_Sbox(S_table(1))
    # l = [0] * 16
    # for x1 in range(64):
    #     x2 = x1 ^ 52
    #     y1, y2 = SBOX(x1), SBOX(x2)
    #     l[y1 ^ y2] += 1
    # print(l)
    
    
    # AES ESQUE BOX
    SBOX = make_aes_esque_sbox()
    table = compute_LAP(SBOX, 12, 4)
    print_LAT_table(table)
    
    
    # Forkbox
    # SBOX = make_fork_Sbox(2)
    # table = compute_LAP(SBOX, 2, 4)
    # print_table(table)
    
    # AND box
    # in_size = 2
    # SBOX = make_AND_Sbox(in_size)
    # table = compute_LAP(SBOX, 2 * in_size, in_size)
    # print_LAT_as_COR_table(table, 2 << in_size)
    
    
    # Example Sbox
    # SBOX = make_example_Sbox([8, 2, 4, 0, 15, 5, 7, 12, 10, 6, 11, 3, 14, 13, 9, 1])
    # table = compute_LAP(SBOX, 4, 4)
    
    # SOme test box
    # for i in range(2):
    #     for j in range(2):
    #         SBOX = make_test_Sbox2(i,j)
    #         table = compute_LAP(SBOX, 2, 2)
    #         print(i,j)
    #         for idx, row in enumerate(table):
    #             print(f'{idx:02X} [{",".join(str(x).rjust(4) for x in row)}]')
    
    # Simon box
    # SBOX = make_SIMON_Sbox()
    # table = compute_LAP(SBOX, 8, 8)
    # for idx, row in enumerate(table):
    #     print(f'{idx:02X} [{",".join(str(x).rjust(4) for x in row)}]')
    
    # # Test table
    # SBOX = make_example_Sbox([8, 2, 4, 0, 15, 5, 7, 12, 10, 6, 11, 3, 14, 13, 9, 1])
    # import random
    # xs = list(range(16))
    # random.shuffle(xs)
    # xs = xs[:8]
    # k1, k2 = 15, 8
    # for x in xs:
    #     print(f'{x}, {SBOX(k1 ^ x) ^k2}')