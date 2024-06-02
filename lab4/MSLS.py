from utils import *
from random_cycles import gen_random_cycles
from local_search import local_search_steepest

def MSLS(distance_matrix, data, n_iter=100):
    start_time = time.time()
    best_cycle_1 = None
    best_cycle_2 = None
    best_length = np.inf

    for i in range(n_iter):
        print(i, end='\r')
        cycle1, cycle2 = gen_random_cycles(get_nodes(data))
        cycle1, cycle2 = local_search_steepest(cycle1, cycle2, distance_matrix, data)
        length = calculate_cycles_length(cycle1, cycle2, distance_matrix)
        
        if length < best_length:
            best_cycle_1 = cycle1
            best_cycle_2 = cycle2
            best_length = length
    stop_time = time.time()
    total_time = stop_time - start_time
    return best_cycle_1, best_cycle_2, total_time