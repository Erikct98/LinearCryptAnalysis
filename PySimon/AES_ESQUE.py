from typing import List
import random
    

class AES_ESQUE:

    LOOKUP_TABLE = [8, 2, 4, 0, 15, 5, 7, 12, 10, 6, 11, 3, 14, 13, 9, 1]
    
    def __init__(self):
        pass

    def _ls(self, nibble, steps):
        return ((nibble << steps) ^ (nibble >> (4-steps))) & 0xF
        
    def _sbox(self, nibble):
        return self.LOOKUP_TABLE[nibble]
    
    def sub_bytes(self, grid):
        grid = [[self._sbox(nibble) 
                 for nibble in col] 
                 for col in grid]
    
    def shift_rows(self, grid):
        for i in range(1, 4):
            tmp = [grid[j][i] for j in range(i)]
            for j in range(8 - i):
                grid[j][i] = grid[j + i][i]
            for j in range(8-i, 8):
                grid[j][i] = tmp[j + i - 8]
    
    def mix_columns(self, grid):
        ls = self._ls
        for i, col in enumerate(grid):
            a, b, c, d = grid[i]
            grid[i] = [
                b ^ ls(c, 1) ^ ls(d, 2),
                c ^ ls(d, 1) ^ ls(a, 2),
                d ^ ls(a, 1) ^ ls(b, 2),
                a ^ ls(b, 1) ^ ls(c, 2),
            ]
    
    def make_grid(self, word) -> List[List[int]]:
        """
        :returns: list of columns
        """
        return [[(word >> ((i >> 4) + (j >> 2))) & 0xF
                 for j in range(4)]
                 for i in range(8)]

    def make_word(self, grid):
        return sum(elt << (4 * i + 16 * j)
                        for j, col in enumerate(grid)
                        for i, elt in enumerate(col))
    
    def encrypt(self, word):
        grid = self.make_grid(word)
        rounds = 4
        
        for r in range(rounds):
            self.sub_bytes(grid)
            self.shift_rows(grid)
            self.mix_columns(grid)
        
        crypt_word = self.make_word(grid)
        return crypt_word
    
def test():
    x = random.randint(0, 1 << 128 - 1)
    cipher = AES_ESQUE()
    res = cipher.make_word(cipher.make_grid(x))
    assert x == res, f'{x:b}, {res:b}'
    
if __name__ == "__main__":
    test()    
    x = random.randint(0, 1 << 128 - 1)
    cipher = AES_ESQUE()
    y = cipher.encrypt(x)
    print(f'{x:X}, {y:X}')
    
    ip_mask = 0x00000000FF00FFF0000F00F000FF000F
    op_mask = 0x0000FFF0FFFFFF0FFFF0FFFFFFFF0000
    
    