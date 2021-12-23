ROUNDS = 27
SET_SIZE = 16

def left_shift_set(set_, shift):
    return set([(elt + SET_SIZE - shift) % SET_SIZE for elt in set_])

def propagate(LHS, RHS):
    return RHS, LHS.symmetric_difference(left_shift_set(RHS, 2))
    
def propagation_correlation(LHS, RHS, rounds):
    trace = [0]
    for i in range(rounds):     
        trace.append(trace[-1] + len(RHS))
        LHS, RHS = propagate(LHS, RHS)
    return trace   
    
if __name__ == "__main__":
    
    # Gather propagations
    results = {}
    for l1 in range(SET_SIZE):
        for l2 in range(l1, SET_SIZE):
            for l3 in range(l2, SET_SIZE):
                for l4 in range(l3, SET_SIZE):
                    for j2 in range(SET_SIZE):
                        for j3 in range(j2, SET_SIZE):
                            for j4 in range(j3, SET_SIZE):
                                LHS = set([l1, l2, l3, l4])
                                RHS = set([0, j2, j3, j4])
                                results.setdefault((str(LHS), str(RHS)), propagation_correlation(LHS.copy(), RHS.copy(), rounds=ROUNDS))

    # Get best scoring propagations
    for i in range(1, ROUNDS):
        try:
            corr_trace = min(results.values(), key=lambda x: x[i])
            item = next(filter(lambda x: x[1] == corr_trace, results.items()))
            print(f"it: {i}, corr=2^-{corr_trace[i]}, mask = {item[0]}")
        except StopIteration:
            print("apparently this does not work...")
        
        
    
    # print(results)
            
    