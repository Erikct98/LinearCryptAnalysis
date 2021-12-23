import random, sys
from Simon import SIMON

WORD_SIZE = 16
assert WORD_SIZE % 8 == 0
key = random.randint(1, 1 << (WORD_SIZE >> 1) - 1)
sim = SIMON(key, 2, WORD_SIZE)

string = 'Hello!'
letters = [ord(l) for l in string]
if len(letters) % (WORD_SIZE / 8):
    letters.append(0)
print(string, letters)

res = []
for a, b in zip(letters[::2], letters[1::2]):
    enc = sim.encrypt((a, b))
    res += enc
    
letters = [x.to_bytes(1, sys.byteorder) for x in res]
print(b''.join(letters))
