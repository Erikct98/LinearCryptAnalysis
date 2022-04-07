from __future__ import annotations
from dataclasses import dataclass
from itertools import product
from math import prod
from typing import Iterable, List, Set

class Sym:

    def can_add(self, __o: object) -> bool:
        raise NotImplementedError("test")

    def __eq__(self, __o: object) -> bool:
        raise NotImplementedError("test")

    def __add__(self, __o: object) -> Sym:
        raise NotImplementedError("test")

    def __mul__(self, __o: object) -> Sym:
        raise NotImplementedError("test")

    def __str__(self) -> str:
        raise NotImplementedError("test")

    def __repr__(self) -> str:
        raise NotImplementedError("test")


@dataclass
class Unary(Sym):
    name: str
    factor: float = 1.0

    @property
    def is_zero(self) -> bool:
        return self.name == 0

    def can_add(self, __o) -> bool:
        return isinstance(__o, Unary) and self.name == __o.name

    def __add__(self, __o):
        if self.can_add(__o):
            return Unary(self.name, self.factor + __o.factor)
        return Sum([self, __o])

    def __mul__(self, o):
        if isinstance(o, (float, int)):
            return Unary(self.name, self.factor * o)
        elif isinstance(o, Unary):
            if o.is_zero:
                return Unary(self.name, self.factor * o.factor)
            if self.is_zero:
                return Unary(o.name, self.factor * o.factor)
            return XOR({Unary(self.name), Unary(o.name)}, factor=self.factor * o.factor)
        else:
            return o * self

    def __str__(self):
        neg = "-" if self.factor < 0 else ""
        fac = f"{abs(self.factor)} * " if abs(self.factor) != 1.0 else ""
        return neg + fac + f"(-1)^{self.name}"

    def __repr__(self):
        return f'<Unary "{self.name}" factor={self.factor}>'

    def __hash__(self):
        return hash(str(self.name) + f"{self.factor}")

    def _simplify(self):
        return self


@dataclass
class XOR(Sym):
    symbols: Set[Unary]
    factor: float = None

    def __post_init__(self):
        self._clean()

    @property
    def name(self):
        return ' + '.join([str(s.name) for s in sorted(self.symbols, key=lambda x: x.name)])

    def can_add(self, __o: object):
        if not isinstance(__o, XOR) or len(self.symbols) != len(__o.symbols):
            return False
        return all(map(lambda x: x[0] == x[1], zip(self.symbols, __o.symbols)))

    def __add__(self, __o):
        assert self.can_add(__o)
        return XOR(self.symbols, self.factor + __o.factor)

    def __mul__(self, o):
        if isinstance(o, (float, int)):
            return XOR(self.symbols, self.factor * o)
        elif isinstance(o, Unary):
            return XOR(self.symbols.symmetric_difference([o]), factor=self.factor * o.factor)
        elif isinstance(o, XOR):
            return XOR(self.symbols.symmetric_difference(o.symbols), factor=self.factor * o.factor)
        else:
            return o * self

    def __str__(self):
        neg = "-" if self.factor < 0 else ""
        fac = f"{abs(self.factor)} * " if abs(self.factor) != 1.0 else ""
        return neg + fac + f"(-1)^({self.name})"

    def __repr__(self):
        return f"<XOR symbols=[{', '.join([repr(s) for s in sorted(self.symbols, key= lambda x: x.name)])}] factor={self.factor}>"

    def _clean(self):
        # Move factor
        if self.factor is None:
            self.factor = prod(x.factor for x in self.symbols)
            self.symbols = [Unary(s.name, 1.0) for s in self.symbols]

        # Remove zeroes
        self.symbols = list(filter(lambda x: not x.is_zero, self.symbols))

        # Cancel duplicates
        symbols = []
        while self.symbols:
            s = self.symbols.pop()
            try:
                self.symbols.remove(s)
                symbols.append(Unary(0))
            except Exception as e:
                symbols.append(s)
        self.symbols = set(symbols)

        # Sort symbols
        self.symbols = set(sorted(self.symbols, key=lambda x: x.name))


    def _simplify(self):
        if len(self.symbols) == 1:
            sym = self.symbols.pop()
            return Unary(sym.name, factor=self.factor)
        if len(self.symbols) == 0:
            return Unary(0, factor=self.factor)
        return self

class Sum(Sym):

    def __init__(self, symbols: Iterable[Unary]):
        self.symbols = list(symbols)
        self._clean()

    def __add__(self, e):
        if isinstance(e, Unary):
            return Sum(self.symbols + [e])
        elif isinstance(e, XOR):
            return Sum(self.symbols + [e])
        elif isinstance(e, Sum):
            return Sum(self.symbols + e.symbols)
        else:
            raise ValueError(f"cannot sum {type(e)}")

    def __mul__(self, e):
        if isinstance(e, (float, int)):
            return Sum([s * e for s in self.symbols])
        elif isinstance(e, (Unary, XOR)):
            return Sum([s * e for s in self.symbols])
        elif isinstance(e, Sum):
            return Sum([s1 * s2 for s1, s2 in product(self.symbols, e.symbols)])
        else:
            raise ValueError("cannot sum")

    def __str__(self):
        min_fac = min(map(lambda x: abs(x.factor), self.symbols))
        prefix = f"1 / {1/min_fac:.0f} * "
        for s in self.symbols:
            s.factor *= 1 / min_fac
        return prefix + f"[{' + '.join([str(s) for s in self.symbols])}]"

    def __repr__(self):
        return f"<SUM symbols=[{', '.join([repr(s) for s in self.symbols])}]>"

    def _clean(self):
        # Remove recursive sums
        sums = [e for e in self.symbols if isinstance(e, Sum)]
        for s in sums:
            self.symbols.remove(s)
            self.symbols.extend(s.symbols)

        # Simplify
        self.symbols = [s._simplify() for s in self.symbols]

        # Combine equivalent elements
        symbols = []
        while self.symbols:
            x = self.symbols.pop()
            for s in self.symbols:
                if x.can_add(s):
                    x += s
                    self.symbols.remove(s)
                    # remove.append(s)
            symbols.append(x)
        self.symbols = symbols

        # Strip elements with zero-factor
        self.symbols = [s for s in self.symbols if s.factor != 0]

        # Sort elements
        unaries = list(filter(lambda x: isinstance(x, Unary), self.symbols))
        xors = list(filter(lambda x: isinstance(x, XOR), self.symbols))
        unaries = sorted(unaries, key=lambda x: str(x.name))
        xors = sorted(xors, key=lambda x: str(x.name))
        self.symbols = unaries + xors

        return


ZERO = Unary(0)


def AND(a: Unary, b: Unary):
    res = Sum([])
    res += ZERO
    res += a
    res += b
    res += a * b * -1
    res *= .5
    return res
    # return Sum([ZERO, a, b, a * b * -1]) * 0.5

def OR(a: Unary, b: Unary):
    res = Sum([])
    res += ZERO * -1
    res += a
    res += b
    res += a * b
    res *= .5
    return res
    # return Sum([ZERO * -1, a, b, a * b]) * 0.5

def test_add():
    a = Unary("A", factor=.5)
    b = Unary("A", factor=1.5)
    c = a + b
    assert c.name == "A" and c.factor == 2.0

def test_add_factor():
    a = Unary("A", factor=.5)
    b = Unary("A", factor=-1.5)
    c = a + b
    assert c.name == "A" and c.factor == -1.0

def test_XOR_0():
    a = Unary("A", factor=.5)
    b = Unary("B", factor=-1.5)
    c = XOR({a, b})
    names = [s.name for s in c.symbols]
    assert "A" in names
    assert "B" in names
    assert c.factor == -0.75

def test_XOR_1():
    a = Unary("A", factor=.5)
    b = Unary("A", factor=-1.5)
    c = XOR({a, b})
    names = [s.name for s in c.symbols]
    assert 0 in names
    assert c.factor == -0.75

def test_XOR_2():
    A = Unary("A")
    B = Unary("B")
    res = XOR([A, B])
    assert str(res) == "1.0 * (-1)^(A + B)", str(res)
    res = XOR([B, A])
    assert str(res) == "1.0 * (-1)^(A + B)", str(res)

def test_AND():
    A = Unary("A")
    B = Unary("B")
    res = AND(A, B)
    assert str(res) == "[0.5 * (-1)^0 + 0.5 * (-1)^A + 0.5 * (-1)^B + -0.5 * (-1)^(A + B)]", str(res)
    res = AND(B, A)
    assert str(res) == "[0.5 * (-1)^0 + 0.5 * (-1)^A + 0.5 * (-1)^B + -0.5 * (-1)^(A + B)]", str(res)

def test_OR():
    A = Unary("A")
    B = Unary("B")
    res = OR(A, B)
    assert str(res) == "[-0.5 * (-1)^0 + 0.5 * (-1)^A + 0.5 * (-1)^B + 0.5 * (-1)^(A + B)]"
    res = OR(B, A)
    assert str(res) == "[-0.5 * (-1)^0 + 0.5 * (-1)^A + 0.5 * (-1)^B + 0.5 * (-1)^(A + B)]"

if __name__ == "__main__":
    test_XOR_1()

    A = Unary("A")
    B = Unary("B")
    C = Unary("C")
    res = OR(AND(A, B), OR(AND(A, C), AND(B, C)))
    print(res)
    # print(repr(outer))
    # print(OR(inner_or, A))
    # print(OR(A, B))

    # res = OR(OR(A, B), C)
    # print(res)
    # print(repr(res))