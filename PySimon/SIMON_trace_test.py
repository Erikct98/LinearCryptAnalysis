from Simon import TEST_SIMON
import random
import threading
import datetime


# Trails
TRAILS = {
    'A': {
        'masks': ([18, 21, 4, 7], [17, 3, 7, 15]),
        'rounds': 6,
        'exp_exp': -8
    },
    'B': {
        'masks': ([16, 2], [18, 0]),
        'rounds': 3,
        'exp_exp': -2
    },
    'C': {
        'masks': ([16, 2], [2]),
        'rounds': 2,
        'exp_exp': -1
    },
    'D': {
        'masks': ([18, 30, 0], [16, 2, 14]),
        'rounds': 5,
        'exp_exp': -4
    },
    'E-FAULTY?': {
        'masks': ([18, 26, 30, 12], [16, 2, 14]),
        'rounds': 7,
        'exp_exp': -7
    },
    'F': {
        'masks': ([18, 26, 30, 12], [28, 2, 10, 14]),
        'rounds': 9,
        'exp_exp': -10
    },
    'G': {
        'masks': ([18, 30, 0], [28, 2, 10, 14]),
        'rounds': 7,
        'exp_exp': -7
    },
    'H': {
        'masks': ([18, 26, 30, 12], [16, 2, 14]),
        'rounds': 7,
        'exp_exp': -9
    },
}

LOCK = threading.Lock()

# Static values
KEY = 8392461815111295945  # random.randint(0, (1 << 64) - 1)
TRAIL_NAME = 'G'
TRAIL = TRAILS[TRAIL_NAME]
NR_ITERATIONS = 1 << (2 * abs(TRAIL['exp_exp']) + 4)
FILENAME = f'correlation_SIMON_ipm={TRAIL["masks"][0]}_opm={TRAIL["masks"][1]}_r={TRAIL["rounds"]}_i={NR_ITERATIONS}_{datetime.datetime.now().strftime("%Y-%m-%d_%H_%M_%S")}.csv'
print(FILENAME)



def make_mask(bits):
    mask = 0
    for bit in bits:
        mask ^= 1 << bit
    return mask


def count_parity_32_bit_word(inp):
    inp ^= inp >> 1
    inp ^= inp >> 2
    inp = (inp & 0x11111111) * 0x11111111
    return (inp >> 28) & 1


def test():
    assert count_parity_32_bit_word(5) == 0
    assert count_parity_32_bit_word(9) == 0
    assert count_parity_32_bit_word(10) == 0
    assert count_parity_32_bit_word(15) == 0
    assert count_parity_32_bit_word(128) == 1
    assert count_parity_32_bit_word((1 << 32) - 1) == 0


def compute_correlation(key: int, iterations: int):
    assert 0 <= key < 1 << 64

    # Setup SIMON
    simon = TEST_SIMON(16, 4, rounds=TRAIL['rounds'])
    simon.set_key(key)

    # Encrypt
    plaintexts = [random.randint(0, (1 << 32) - 1) for _ in range(iterations)]
    cryptexts = [simon.encrypt(pt) for pt in plaintexts]

    # Compute correlation
    masks = TRAIL['masks']
    IPM, OPM = make_mask(masks[0]), make_mask(masks[1])
    count = sum(count_parity_32_bit_word((pt & IPM) ^ (ct & OPM)) for pt, ct in zip(plaintexts, cryptexts))
    p = 1 - count / NR_ITERATIONS
    corr = 2*p - 1

    with LOCK:
        with open(FILENAME, 'a+') as f:
            f.write(f'{key};{corr}\n')

def make_thread():
    key = random.randint(0, (1 << 64) - 1)
    return threading.Thread(
            target=compute_correlation,
            args=(key, NR_ITERATIONS)
        )

# Perform the necessary encryptions.
if __name__ == "__main__":
    test()

    threads = list()
    for j in range(10):
        x = make_thread()
        threads.append(x)
        x.start()

    for i in range(1000):
        threads[i].join()
        x = make_thread()
        threads.append(x)
        x.start()

    for i in range(1000, 1010):
        threads[i].join()

    # Define masks
    # masks = TRAIL['masks']
    # IPM, OPM = make_mask(masks[0]), make_mask(masks[1])

    # for i in range(1000):
    #     # Encrypt
    #     plaintexts = [random.randint(0, (1 << 32) - 1) for _ in range(NR_ITERATIONS)]
    #     cryptexts = [simon.encrypt(pt) for pt in plaintexts]

    #     # Count results
    #     count = sum(count_parity_32_bit_word((pt & IPM) ^ (ct & OPM)) for pt, ct in zip(plaintexts, cryptexts))
    #     p = 1 - count / NR_ITERATIONS
    #     corr = 2*p - 1
    #     exp = math.log(abs(corr), 2)
    #     print(f'{p=:.4f}, {corr=:.4f}, {exp=:.4f}')

    #     # # Get key bit val
    #     # subkey = KEY & ((1 << 16) - 1)
    #     # key_mask = make_mask([1, 3, 5, 6, 7])
    #     # print("ip", ip(subkey, key_mask, 16))
