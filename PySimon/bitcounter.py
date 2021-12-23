import random

def test(range_, cnt):
    res = {
        'xcnt & ycnt == 0': 0,
        'xcnt & ycnt == xcnt': 0,
        'xcnt & ycnt == ycnt': 0,
        'xcnt ^ ycnt == 1': 0
    }
    for i in range(cnt):
        x = random.choice(range_)
        y = random.choice(range_)
        z = x & y
        xstr, ystr, zstr = f'{x:b}', f'{y:b}', f'{z:b}'
        xcnt, ycnt, zcnt = xstr.count('1') % 2, ystr.count('1') % 2, zstr.count('1') % 2
        # print(f"{x} & {y} = {z} | {xstr:0>5} & {ystr:0>5} = {zstr:0>5} | {xcnt} & {ycnt} = {zcnt}")
        for key in res:
            if eval(key):
                res[key] += 1
        
    for key, val in res.items():
        print(f'{key}: {val / cnt}')

if __name__ == "__main__":
    test(range(0, 16), 10000)