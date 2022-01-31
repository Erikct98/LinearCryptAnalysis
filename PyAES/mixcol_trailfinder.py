class Byyte:
    def __init__(self, symbols=None):
        if symbols is None:
            symbols = [[] for _ in range(8)]
        self.symbols = symbols

    def mulp(self, s: int):
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

        # Clean
        # TODO

    @classmethod
    def concat(cls, os: 'Byyte') -> 'Byyte':
        clones = [x.clone() for x in os]
        for i in clones[1:]:
            clones[0].symbols.extend(i.symbols)
        return clones[0]

    def __repr__(self):
        return f"<class 'Byytes', symbols={repr(self.symbols)}>"

def fwd_map() :
    ip = [Byyte([[(id_, i)] for i in range(7, -1, -1)]) for id_ in range(4)]
    return Byyte.concat([ip[i].mulp(2).add(ip[(i + 1) % 4].mulp(3)).add(ip[(i + 2) % 4]).add(ip[(i + 3) % 4]) for i in range(4)])

def bwd_map():
    ip = [Byyte([[(id_, i)] for i in range(7, -1, -1)]) for id_ in range(4)]
    return Byyte.concat([ip[i].mulp(14).add(ip[(i + 1) % 4].mulp(11)).add(ip[(i + 2) % 4].mulp(13)).add(ip[(i + 3) % 4].mulp(9)) for i in range(4)])

def find_input_mask(opm):
    byytes = fwd_map()
    indices = [idx for idx, elt in enumerate(f'{opm:0>32b}') if elt == "1"]
    symbs = [x for idx in indices for x in byytes.symbols[idx]]
    ipm = 0
    for word, idx in symbs:
        ipm ^= 1 << ((3-word) * 8 + idx)
    return ipm

def find_output_mask(ipm):
    byytes = bwd_map()
    indices = [idx for idx, elt in enumerate(f'{ipm:0>32b}') if elt == "1"]
    symbs = [x for idx in indices for x in byytes.symbols[idx]]
    opm = 0
    for word, idx in symbs:
        opm ^= 1 << ((3-word) * 8 + idx)
    return opm

if __name__ == "__main__":
    assert find_input_mask(0x80000000) == 0x40C08080
    assert find_output_mask(0x40C08080) == 0x80000000
    print(f'{find_input_mask(0x00010000):0>8X}')