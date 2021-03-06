"""
This library provides means to compute linear trails through the
MixColumns step that hold with corr = 1.
"""
from typing import List, Tuple


Symbols = List[List[Tuple[int, int]]]


class Byyte:
    """
    Symbolic representation of a byte. Used in generating linear trails
    through the MixColumn step.
    """

    def __init__(self, symbols: Symbols = None):
        if symbols is None:
            symbols = [[] for _ in range(8)]
        self.symbols = symbols

    def sm(self, s: int):
        """
        Return `s` * self.
        """
        copy = self.clone()

        res = Byyte(None)
        while s:
            if s & 1:
                res.add(copy)
            s >>= 1
            copy.mulp2()
        return res

    def mulp2(self):
        head, self.symbols = self.symbols[0], self.symbols[1:]
        self.symbols.append(head)
        self.symbols[3].extend(head)
        self.symbols[4].extend(head)
        self.symbols[6].extend(head)

    def clone(self):
        return Byyte([[x for x in s] for s in self.symbols])

    def add(self, o: 'Byyte'):
        for i in range(8):
            self.symbols[i].extend(o.symbols[i])
        return self

    @classmethod
    def sum(cls, os: List['Byyte']) -> 'Byyte':
        """
        Sum multiple Byyte objects
        """
        res = os[0].clone()
        for o in os[1:]:
            res.add(o)
        return res

    @classmethod
    def concat(cls, os: List['Byyte']) -> 'Byyte':
        """
        Concatenate Byyte objects.
        """
        clones = [x.clone() for x in os]
        for i in clones[1:]:
            clones[0].symbols.extend(i.symbols)
        return clones[0]

    def __repr__(self):
        return f"<class 'Byytes', symbols={repr(self.symbols)}>"


def bwd_map() -> Byyte:
    """
    Computes the linear trail in the backward direction.
    I.e. every input bit is given as linear combination of output bits.
    """
    ip = [Byyte([[(id_, i)] for i in range(7, -1, -1)]) for id_ in range(4)]
    return Byyte.concat([Byyte.sum([ip[i].sm(2), ip[(i + 1) % 4].sm(3), ip[(i + 2) % 4], ip[(i + 3) % 4]]) for i in range(4)])


def fwd_map() -> Byyte:
    """
    Computes the linear trail in the forward direction.
    I.e. every output bit is given as linear combination of input bits.
    """
    ip = [Byyte([[(id_, i)] for i in range(7, -1, -1)]) for id_ in range(4)]
    return Byyte.concat([Byyte.sum([ip[i].sm(14), ip[(i + 1) % 4].sm(11), ip[(i + 2) % 4].sm(13), ip[(i + 3) % 4].sm(9)]) for i in range(4)])


def find_input_mask(opm: int) -> int:
    """
    Compute input mask that has correlation 1 with `opm` through MixColumns.
    """
    byytes = bwd_map()
    indices = [idx for idx, elt in enumerate(f'{opm:0>32b}') if elt == "1"]
    symbs = [x for idx in indices for x in byytes.symbols[idx]]
    ipm = 0
    for word, idx in symbs:
        ipm ^= 1 << ((3-word) * 8 + idx)
    return ipm


def find_output_mask(ipm: int) -> int:
    """
    Compute output mask that has correlation 1 with `ipm` through MixColumns.
    """
    byytes = fwd_map()
    indices = [idx for idx, elt in enumerate(f'{ipm:0>32b}') if elt == "1"]
    symbs = [x for idx in indices for x in byytes.symbols[idx]]
    opm = 0
    for word, idx in symbs:
        opm ^= 1 << ((3-word) * 8 + idx)
    return opm


def test_find_input_mask():
    assert find_input_mask(0x80000000) == 0x40C08080


def test_find_output_mask():
    assert find_output_mask(0x40C08080) == 0x80000000


if __name__ == "__main__":
    pass
