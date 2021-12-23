from .Sbox import SIMONbox

class TestCyclicShift:
    
    def test_shift_by_nothing(self):
        box = SIMONbox(in_size=8)
        inp = 0x39
        assert box.cyclic_left_shift(inp, 0) == inp

    def test_shift_by_one(self):
        box = SIMONbox(in_size=8)
        inp = 0x38
        assert box.cyclic_left_shift(inp, 1) == inp << 1
        
        inp = 0x89
        assert box.cyclic_left_shift(inp, 1) == 0x13

    def test_shift_by_two(self):
        box = SIMONbox(in_size=4)
        inp = 0x8
        assert box.cyclic_left_shift(inp, 2) == 0x2
        
        inp = 0xB
        assert box.cyclic_left_shift(inp, 2) == 0xE
        
    def test_shift_by_five(self):
        box = SIMONbox(in_size=8)
        inp = 0x52
        assert box.cyclic_left_shift(inp, 5) == 0x4A
        
        inp = 0x7D
        assert box.cyclic_left_shift(inp, 5) == 0xAF