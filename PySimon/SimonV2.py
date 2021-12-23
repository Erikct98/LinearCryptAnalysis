# Global variables
N = 16
M = 4


###############################################################################
# Verify (N,M) is a supported pair
NM_pairs = [(16,4), (24,3), (24, 4), (32, 3), (32, 4), (48, 2), (48, 3),
            (128, 2), (128, 3), (128, 4)]
assert (N,M) in NM_pairs

# Compute (T,J) corresponding to the (N,M) pair
TJ_map = {
    (16,4): (32,0),
    (24,3): (36,0),
    (24, 4): (36,1),
    (32, 3): (42,0),
    (32, 4): (44,3),
    (48, 2): (52,2),
    (48, 3): (54,3),
    (128, 2): (68,2),
    (128, 3): (69,3),
    (128, 4): (72,4)
}
T, J = TJ_map[(N,M)]


# Z-bits
Z = [
    [1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0],
    [1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0],
    [1,0,1,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,0,0,1,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,1,1],
    [1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,0,0,1,1,1,1],
    [1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,1,1,1]
]

C = (1 << N) - 4 # 2^n - 4

# Other useful values:
N_BIT_MASK = (1 << N) - 1
TWO_N_BIT_MASK = (1 << (2 * N)) - 1


def cylesh(word, steps):
    """
    Cyclic Left Shift
    """
    assert 0 <= word <= N_BIT_MASK
    return ((word << steps) ^ (word >> (N - steps))) & N_BIT_MASK

def cyrish(word, steps):
    """
    Cyclic Right Shift
    """
    assert 0 <= word <= N_BIT_MASK
    return ((word >> steps) ^ (word << (N - steps))) & N_BIT_MASK

def get_subkeys(key):
    assert 0 <= key <= (1 << N * M)
    
    k = []
    
    for i in range(M):
        subkey = (key >> (i * N)) & N_BIT_MASK
        k.append(subkey)
    
    for i in range(M, T):
        tmp = cyrish(k[i - 1], 3)
        if M == 4: tmp ^= k[i-3]
        tmp ^= cyrish(tmp, 1)
        
        new_key = ~k[i-M] ^ tmp ^ Z[J][(i - M) % 62] ^ 3
        k.append(new_key & N_BIT_MASK)
    
    return k

def encrypt(plaintext, key):
    assert 0 <= plaintext <= TWO_N_BIT_MASK
    x,y = plaintext >> N, plaintext & N_BIT_MASK
    
    keys = get_subkeys(key)
    for i in range(T):
        x, y = y ^ (cylesh(x, 1) & cylesh(x, 8)) ^ cylesh(x, 2) ^ keys[i], x

    crypt_word = (x << N) ^ y
    return crypt_word

print(get_subkeys(0x1918111009080100))

def test():
    # SIMON 32/64 testvector
    key = 0x1918111009080100
    pt, cp = 0x65656877, 0xc69be9bb
    res = encrypt(pt, key)
    print(f'0x{res:X}, 0x{cp:X}')
    assert res == cp, f'{res:X}'

test()