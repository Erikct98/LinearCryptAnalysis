class TEST_SIMON:

    CONFIG = {
        (16, 4): (32, 0),
        (24, 3): (36, 0),
        (24, 4): (36, 1),
        (32, 3): (42, 2),
        (32, 4): (44, 3),
        (48, 2): (52, 2),
        (48, 3): (54, 3),
        (64, 2): (68, 2),
        (64, 3): (69, 3),
        (64, 4): (72, 4)
    }

    Z = [
        [1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0],
        [1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0],
        [1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1],
        [1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1],
        [1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1]
    ]

    def __init__(self, n, m, master_key=None, rounds=None):
        assert (n, m) in self.CONFIG, (n,m)

        self.N = n
        self.M = m
        self.block_size = 2 * n
        self.key_size = m * n
        self.T, self.J = self.CONFIG[(n, m)]
        if rounds:
            self.T = rounds

        self.N_BIT_MASK = (1 << n) - 1
        self.TWO_N_BIT_MASK = (1 << (2 * n)) - 1

    def cylesh(self, word, steps):
        """
        Cyclic Left Shift
        """
        # assert 0 <= word <= self.N_BIT_MASK
        return ((word << steps) ^ (word >> (self.N - steps))) & self.N_BIT_MASK

    def cyrish(self, word, steps):
        """
        Cyclic Right Shift
        """
        # assert 0 <= word <= self.N_BIT_MASK
        return ((word >> steps) ^ (word << (self.N - steps))) & self.N_BIT_MASK

    def set_key(self, key):
        assert 0 < key < 1 << self.key_size
        self._key = key
        self._subkeys = self.get_subkeys()

    def get_subkeys(self):
        k = []

        for i in range(self.M):
            subkey = (self._key >> (i * self.N)) & self.N_BIT_MASK
            k.append(subkey)

        for i in range(self.M, self.T):
            tmp = self.cyrish(k[i - 1], 3)
            if self.M == 4:
                tmp ^= k[i-3]
            tmp ^= self.cyrish(tmp, 1)
            new_key = ~k[i-self.M] ^ tmp ^ self.Z[self.J][(i - self.M) % 62] ^ 3
            k.append(new_key & self.N_BIT_MASK)

        return k

    def encrypt(self, plaintext):
        # assert 0 <= plaintext <= self.TWO_N_BIT_MASK
        x, y = plaintext >> self.N, plaintext & self.N_BIT_MASK

        keys = self._subkeys
        for i in range(self.T):
            x, y = y ^ (self.cylesh(x, 1) & self.cylesh(x, 8)) ^ self.cylesh(x, 2) ^ keys[i], x

        crypt_word = (x << self.N) ^ y
        return crypt_word


def test_class():
    tests = [
        (16, 4, 0x1918111009080100,0x65656877,0xc69be9bb),
        (24, 3, 0x1211100a0908020100,0x6120676e696c,0xdae5ac292cac),
        (24, 4, 0x1a19181211100a0908020100,0x72696320646e,0x6e06a5acf156),
        (32, 3, 0x131211100b0a090803020100,0x6f7220676e696c63,0x5ca2e27f111a8fc8),
        (32, 4, 0x1b1a1918131211100b0a090803020100,0x656b696c20646e75,0x44c8fc20b9dfa07a),
        (48, 2, 0x0d0c0b0a0908050403020100,0x2072616c6c69702065687420,0x602807a462b469063d8ff082),
        (48, 3, 0x1514131211100d0c0b0a0908050403020100,0x74616874207473756420666f,0xecad1c6c451e3f59c5db1ae9),
        (64, 2, 0x0f0e0d0c0b0a09080706050403020100,0x63736564207372656c6c657661727420,0x49681b1e1e54fe3f65aa832af84e0bbc),
        (64, 3, 0x17161514131211100f0e0d0c0b0a09080706050403020100,0x206572656874206e6568772065626972,0xc4ac61effcdc0d4f6c9c8d6e2597b85b),
        (64, 4, 0x1f1e1d1c1b1a191817161514131211100f0e0d0c0b0a09080706050403020100,0x74206e69206d6f6f6d69732061207369,0x8d2b5579afc8a3a03bf72a87efe7b868)
    ]

    for n, m, key, pt, ct in tests:
        simon = TEST_SIMON(n, m)
        simon.set_key(key)
        assert simon.encrypt(pt) == ct

test_class()
