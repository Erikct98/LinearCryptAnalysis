"""
This library provides tools that are useful in trailing through AES in case
of non-zero keys and extracting keybits in the process.
"""
from correlation import keyed_func_corr
from linearization import keyed_linearization_for_ipm
from toolbox import I, J, P8, GF2_8
from collections import Counter
from LAT import LAT


def compute_correlations_wo_key(key_guess):
    results = [list() for _ in GF2_8]
    for ipm in GF2_8:
        for key in GF2_8:
            def ipf(pt): return P8(pt & ipm)
            opf = keyed_linearization_for_ipm(ipm, key_guess)
            results[ipm].append(keyed_func_corr(ipf, opf, 0x0, key))
        print(results[ipm], Counter(results[ipm]))
    return results

def KAT(guess: int, key:int, opm:int) -> int:
    """
    Key Approximation Table
    :param guess: guessed key
    :param key: actual encryption key
    :param opm: output mask.
    :returns: correlation between guess and key for opm.
    """
    GAT = [LAT[ipm][opm] * I(P8(ipm & guess)) for ipm in GF2_8]
    KAT = [LAT[ipm][opm] * I(P8(ipm & key)) for ipm in GF2_8]
    corr128 = 128
    for pt in GF2_8:
        exp_par = J(sum(GAT[ipm] * I(P8(ipm & pt)) for ipm in GF2_8) >> 7)
        act_par = J(sum(KAT[ipm] * I(P8(ipm & pt)) for ipm in GF2_8) >> 7)
        corr128 -= exp_par ^ act_par
    return corr128


def compute_key_byte(a):
    total = sum(map(lambda x: abs(x[1]), a))
    counts = [sum(map(lambda x : int((f'{x[0]:08b}')[i]) * x[1], a)) - total/2 for i in range(8)]
    print(counts)
    return int(''.join([str(int(counts[i] >= 0)) for i in range(8)]), 2)


if __name__ == "__main__":
    x = [(0, -62), (1, 69), (2, 121), (3, 34), (4, -7), (5, 155), (6, -100), (7, -186), (8, -31), (9, 26), (10, -188), (11, 43), (12, 150), (13, 193), (14, 32), (15, -104), (16, -66), (17, -168), (18, 19), (19, -110), (20, -66), (21, 117), (22, -20), (23, 42), (24, 354), (25, 182), (26, -19), (27, 15), (28, 169), (29, 106), (30, 14), (31, 66), (32, 141), (33, 98), (34, -163), (35, -126), (36, -244), (37, -62), (38, 160), (39, 221), (40, -32), (41, -74), (42, 130), (43, -73), (44, 61), (45, 96), (46, -282), (47, -67), (48, 298), (49, 41), (50, -144), (51, -225), (52, -105), (53, -142), (54, 52), (55, 54), (56, -60), (57, -60), (58, -35), (59, -63), (60, -61), (61, -126), (62, 48), (63, 132), (64, -255), (65, -238), (66, 88), (67, -144), (68, 220), (69, 83), (70, -72), (71, -137), (72, 199), (73, 87), (74, 86), (75, -71), (76, -181), (77, -34), (78, 125), (79, -188), (80, -238), (81, 47), (82, 91), (83, 337), (84, -242), (85, 156), (86, -162), (87, -110), (88, 143), (89, 6), (90, -33), (91, -4), (92, -167), (93, 120), (94, -113), (95, -5), (96, 0), (97, 99), (98, 47), (99, 82), (100, -297), (101, -52), (102, -116), (103, 36), (104, -19), (105, 53), (106, 63), (107, 231), (108, -39), (109, -8), (110, 188), (111, -35), (112, 136), (113, -99), (114, 160), (115, -82), (116, 20), (117, 153), (118, -50), (119, 217), (120, -1), (121, 69), (122, -59), (123, -35), (124, -230), (125, -197), (126, 228), (127, -121), (128, -90), (129, -64), (130, -3), (131, -227), (132, 144), (133, -74), (134, 25), (135, -121), (136, 161), (137, 48), (138, 118), (139, 27), (140, -5), (141, 33), (142, 218), (143, 79), (144, -172), (145, 149), (146, -48), (147, 196), (148, -138), (149, 10), (150, -105), (151, 94), (152, -52), (153, 194), (154, -51), (155, 92), (156, -60), (157, -37), (158, 61), (159, 103), (160, -58), (161, -144), (162, -44), (163, 247), (164, -171), (165, 50), (166, -201), (167, -66), (168, -48), (169, 31), (170, 93), (171, 47), (172, 172), (173, 147), (174, -6), (175, 1), (176, 156), (177, 109), (178, -48), (179, 22), (180, 114), (181, 54), (182, 118), (183, -98), (184, -136), (185, -165), (186, -68), (187, 41), (188, -161), (189, -55), (190, -129), (191, -45), (192, -40), (193, 202), (194, -182), (195, -53), (196, -174), (197, -49), (198, 128), (199, 21), (200, -47), (201, 35), (202, 26), (203, 185), (204, -215), (205, -144), (206, 6), (207, -94), (208, -239), (209, -220), (210, -56), (211, 58), (212, -195), (213, -148), (214, 408), (215, -190), (216, 121), (217, 60), (218, 178), (219, -60), (220, 179), (221, -139), (222, 125), (223, -2), (224, 93), (225, -187), (226, -11), (227, 135), (228, -91), (229, 184), (230, 286), (231, 188), (232, -86), (233, -56), (234, 135), (235, -61), (236, -22), (237, -82), (238, -88), (239, 9), (240, 105), (241, 105), (242, 50), (243, 160), (244, 55), (245, 20), (246, -112), (247, -71), (248, -108), (249, 21), (250, 155), (251, 19), (252, -170), (253, -86), (254, 108), (255, -250)]
    y = sorted(x, key=lambda x: abs(x[1]))
    a = [(x, abs(y)) for x, y in y]
    key = compute_key_byte(a)
    print(f'key = {key:3} = 0x{key:2X} = 0b{key:08b}')

    # print(compute_correlations_wo_key(0xB3))
    # print(KAT(0, 0x07, 0x01))
