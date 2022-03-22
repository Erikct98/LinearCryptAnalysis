import json
from typing import Dict, Iterable, List

# Find basis for keys
def find_XOR_basis(elements: Iterable[int]) -> List[int]:
    """
    Finds a basis for `elements`
    Assumes elements actually forms a basis.
    """
    basis = set()
    seen = set([0])
    for key in elements:
        if key not in seen:
            basis.add(key)
        seen.update([e ^ key for e in seen])
        seen.add(key)
    return sorted(basis)

if __name__ == "__main__":
    with open("../AES/kg_per_opm.json", 'r') as fp:
        data: Dict[str, Dict[str, str]] = json.load(fp)

    reverse_mapping = {}
    for opm, keys in data.items():
        # Invert key -> key guess mapping.
        rm = {}
        for key, mask in keys.items():
            rm.setdefault(mask, [])
            rm[mask].append(key)
        reverse_mapping[opm] = rm
    # print(reverse_mapping)

    # Check bits in mask are same as keys
    for opm, keys in reverse_mapping.items():
        for mask, keys in keys.items():
            x = [i for i, elt in enumerate(mask) if elt == "1"]
            k = [int(i) for i in keys]
            assert all(i == j for i, j in zip(x, k)), (x, k)

    # Convert reverse mapping
    for opm in reverse_mapping:
        reverse_mapping[opm] = [[int(k) for k in keys] for keys in reverse_mapping[opm].values()]
    # print(reverse_mapping)


    # Check for basis
    for opm, key_groups in reverse_mapping.items():
        group = key_groups[0]
        for k1 in group:
            for k2 in group:
                assert k1 ^ k2 in group

    # Make basis
    basis_mapping = {}
    for opm, key_groups in reverse_mapping.items():
        group = key_groups[0]
        basis = find_XOR_basis(group)
        ortho = find_XOR_basis([g[0] for g in key_groups])
        basis_mapping[opm] = {"basis": basis, "orthogonal": ortho}

    # Convert notation
    basis_mapping = {f"0x{int(opm):02X}" : {k: [f"0x{i:02X}" for i in v] for k,v in info.items()} for opm, info in basis_mapping.items()}

    with open("key_guess_mapping_v5.json", "w") as fp:
        json.dump(basis_mapping, fp)

# for opm in GF2_8:
#     print(opm, [i for i, e in enumerate(ocorr(opm)) if abs(e) == 16])