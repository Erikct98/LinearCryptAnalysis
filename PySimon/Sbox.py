from abc import ABC

class Sbox(ABC):
    
    def encrypt(self, *c: int) -> int:
        pass


class DESbox(Sbox):
    
    def __init__(self, lookup_table, in_size: int):
        self.max_in_val = 1 << in_size
        self.table = lookup_table
    
    def encrypt(self, word: int) -> int:
        assert 0 <= word < self.max_in_val
        return self.table[word]

class SIMONbox(Sbox):
    
    def __init__(self, in_size: int):
        self.in_size = in_size
        self.max_in_val = 1 << in_size
        
    def cls(self, word, ticks):
        """
        CLS: cyclic left shift
        """
        shifted = word << ticks
        return (shifted & (self.max_in_val - 1)) ^ (shifted >> self.in_size)
    
    def encrypt(self, word):
        assert 0 <= word < self.max_in_val
        L = self.cls(word, 1)
        R = self.cls(word, 8)
        return L & R
        