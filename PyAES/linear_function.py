from typing import Dict, Callable
from toolbox import I, P8


class LinearFunction:

    def __init__(self, terms: Dict[int, int]):
        self.terms = terms
        self._func = self._make_func(terms)

    def _make_func(self, terms) -> Callable:
        def _func(input: int) -> bool:
            return sum(count * I(P8(input & mask)) for mask, count in terms.items()) >= 0
        return _func

    def __call__(self, input: int) -> bool:
        return self._func(input)

    def __str__(self) -> str:
        terms = [f"{count:2} * I(P8(i & 0x{mask:2X}))"
                 for mask, count in self.terms.items()]
        return f"({' + '.join(terms)}) >= 0"
