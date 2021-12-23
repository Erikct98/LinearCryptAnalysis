fn = "output.txt"

with open(fn, 'r') as f:
    lines = f.readlines()
    
rows = []
for line in lines:
    ln, row = line.split(' ',1)
    rows.append(list(map(lambda x: int(x.strip(' []\n')) if x else 0, row.strip('[]').split(','))))

# indices = []
# for ridx, row in enumerate(rows):
#     r_indices = []
#     try:
#         start = 0
#         while start < len(row):
#             idx = row[start:].index(64)
#             r_indices.append(idx)
#             start = idx
#     except ValueError:
#         pass
#     try:
#         start = 0
#         while start < len(row):
#             idx = row[start:].index(-64)
#             r_indices.append(idx)
#             start = idx
#     except ValueError:
#         pass
#     for cidx in r_indices:
#         indices.append((ridx, cidx))
# print(indices)

indices = {}
for ridx, row in enumerate(rows):
    for cidx, elt in enumerate(row):        
        if elt:
            if elt not in indices:
                indices[elt] = []
            indices[elt].append((f'{ridx:X}', f'{cidx:X}'))
print(indices)