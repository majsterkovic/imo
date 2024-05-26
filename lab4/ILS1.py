from utils import *
from random_cycles import gen_random_cycles
from local_search import local_search_steepest

def little_perturbation(cycle1, cycle2):
    for cycle in [cycle1, cycle2]:
        a = np.random.choice(cycle, 2, replace=False)
        a_idx_a = np.where(cycle==a[0])
        a_idx_b = np.where(cycle==a[1])
        
        tmp = cycle[a_idx_a]
        cycle[a_idx_a] = cycle[a_idx_b]
        cycle[a_idx_b] = tmp
    
    a = np.random.choice(cycle1, 1)
    a_idx = np.where(cycle1==a)
    
    b = np.random.choice(cycle2, 1)
    b_idx = np.where(cycle2==b)

    tmp = cycle1[a_idx]
    cycle1[a_idx] = cycle2[b_idx]
    cycle2[b_idx] = tmp

    return cycle1, cycle2

def ILS1(distance_matrix, data, time_MSLS):
    best_cycle_1, best_cycle_2 = gen_random_cycles(get_nodes(data))
    best_cycle_1, best_cycle_2 = local_search_steepest(best_cycle_1, best_cycle_2, distance_matrix, data)
    best_length = calculate_cycles_length(best_cycle_1, best_cycle_2, distance_matrix)

    time_elapsed = 0

    while time_elapsed <= time_MSLS:
        start_time = time.time()
        cycle1, cycle2 = little_perturbation(best_cycle_1, best_cycle_2)
        cycle1, cycle2 = local_search_steepest(cycle1, cycle2, distance_matrix, data)
        length = calculate_cycles_length(cycle1, cycle2, distance_matrix)
        if length < best_length:
            best_length = length
            best_cycle_1 = cycle1
            best_cycle_2 = cycle2
        stop_time = time.time()
        time_elapsed += (stop_time - start_time)
    
    return best_cycle_1, best_cycle_2