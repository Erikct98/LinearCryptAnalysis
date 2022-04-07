from dataclasses import dataclass
from typing import List, Set, Union
from itertools import product


@dataclass()
class Sym:
    name: Union[str, int]

    def __hash__(self):
        return hash(self.name)

    def clone(self):
        return Sym(self.name)

    def __repr__(self):
        return f"Sym('{self.name}')"

    def export(self):
        return str(self.name)

    def __lt__(self, __o):
        return str(self.name) < str(__o.name)

Z = Sym(0)


@dataclass
class Exp:
    syms: Set[Sym]
    fact: float = 1.0

    def __post_init__(self):
        assert isinstance(self.syms, set), self.syms
        assert isinstance(self.fact, (float, int))

    def can_add(self, __o):
        return isinstance(__o, Exp) and self.syms == __o.syms

    def clone(self):
        return Exp({s.clone() for s in self.syms}, self.fact)

    def __add__(self, __o):
        assert self.can_add(__o)
        res = self.clone()
        res.fact += __o.fact
        return res

    def __mul__(self, __o):
        if isinstance(__o, (float, int)):
            res = self.clone()
            res.fact *= __o
            return res
        elif isinstance(__o, Exp):
            syms = {s.clone()
                    for s in self.syms.symmetric_difference(__o.syms)}
            return Exp(syms, self.fact * __o.fact)
        elif isinstance(__o, ExpSum):
            return __o * self
        else:
            raise NotImplementedError("woops")

    def __repr__(self):
        return f'Exp(syms={self.syms}, fact={self.fact})'

    def simplify(self):
        # Deal with empty case
        if len(self.syms) == 0:
            return expZ * self.fact

        # Remove zero element
        if len(self.syms) > 1:
            syms = {s.clone() for s in self.syms if s.name != Z.name}
            return Exp(syms, self.fact)
        return self.clone()

    def export(self, fac):
        min = "- " if self.fact < 0 else ""
        fact = self.fact / fac
        fact = f"{abs(fact):.2f}" if abs(fact) != 1 else ""
        return min + fact + f"(-1)^({' + '.join([s.export() for s in sorted(self.syms)])})"


expZ = Exp({Z})


@dataclass
class ExpSum:
    exps: List[Exp]

    def __post_init__(self):
        # Remove recursive sums
        exps = []
        for e in self.exps:
            if isinstance(e, Exp):
                exps.append(e)
            else:
                exps.extend(e.exps)
        self.exps = exps

    def __add__(self, __o):
        if isinstance(__o, Exp):
            exps = [s for s in self.exps] + [__o]
        elif isinstance(__o, ExpSum):
            exps = [s for s in self.exps] + [s for s in __o.exps]
        exps = self.simplify(exps)
        return ExpSum(exps)

    def __mul__(self, __o):
        if isinstance(__o, (float, int, Exp)):
            exps = [s * __o for s in self.exps]
        elif isinstance(__o, ExpSum):
            exps = [s1 * s2 for s1, s2 in product(self.exps, __o.exps)]
        else:
            raise NotImplementedError("woops")
        exps = self.simplify(exps)
        return ExpSum(exps)

    @staticmethod
    def simplify(exps: List[Exp]) -> List[Exp]:
        # Combine duplicate elements
        new_exps: List[Exp] = []
        while exps:
            e1 = exps.pop()
            for e2 in exps:
                if e1.can_add(e2):
                    e1 = e1 + e2
                    exps.remove(e2)
            new_exps.append(e1)

        # Simplify all elements
        new_exps = [s.simplify() for s in new_exps]
        return new_exps

    def export(self):
        min_fac = min(map(lambda x: abs(x.fact), self.exps))
        elts = [e.export(min_fac) for e in self.exps]
        elts = sorted(elts)
        elts = elts[0:1] + ["+ " + elt if not elt.startswith("-") else elt for elt in elts[1:]]
        return f"1/{1/min_fac:.2f} * [{' '.join(elts)}]"


def OR(a: Exp, b: Exp) -> ExpSum:
    return ExpSum([expZ * -1, a, b, a * b]) * .5


def AND(a: Exp, b: Exp) -> ExpSum:
    return ExpSum([expZ, a, b, a * b * -1]) * .5


if __name__ == "__main__":
    A = Sym("A")
    B = Sym("B")
    C = Sym("C")
    expA = Exp({A})
    expB = Exp({B})
    expC = Exp({C})
    x = OR(expA, expB)
    res = OR(expC, x)
    res = res.export()
    print(res)
