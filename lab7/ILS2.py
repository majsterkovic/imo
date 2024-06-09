from utils import *
from random_cycles import gen_random_cycles
from local_search import local_search_steepest
from copy import deepcopy


def severe_perturbation(cycle1, cycle2, distance_matrix, size=0.05):

    # copy cycles
    cycle1_copy = deepcopy(cycle1)
    cycle2_copy = deepcopy(cycle2)

    random1 = list(np.random.choice(cycle1, int(len(cycle1) * size), replace=False))
    cycle1 = [node for node in cycle1 if node not in random1]

    random2 = list(np.random.choice(cycle2, int(len(cycle2) * size), replace=False))
    cycle2 = [node for node in cycle2 if node not in random2]


    free_nodes = random1 + random2
    if len(free_nodes) == 200:
        return np.array(cycle1_copy), np.array(cycle2_copy)

    while len(cycle1) < 100:
        best_update1 = float('inf')
        best_node1 = None
        best_position1 = -1

        for node in free_nodes:
            for i in range(len(cycle1)):
                distance_update = distance_matrix[cycle1[i-1]][node] + distance_matrix[node][cycle1[i]] - distance_matrix[cycle1[i-1]][cycle1[i]]
                if distance_update < best_update1:
                    best_update1 = distance_update
                    best_node1 = node
                    best_position1 = i

        if best_node1 is not None:
            cycle1.insert(best_position1, best_node1)
            free_nodes.remove(best_node1)
    
    while len(free_nodes) > 0:
        best_update2 = float('inf')
        best_node2 = None
        best_position2 = -1

        for node in free_nodes:
            for i in range(len(cycle2)):
                distance_update = distance_matrix[cycle2[i-1]][node] + distance_matrix[node][cycle2[i]] - distance_matrix[cycle2[i-1]][cycle2[i]]
                if distance_update < best_update2:
                    best_update2 = distance_update
                    best_node2 = node
                    best_position2 = i

        if best_node2 is not None:
            cycle2.insert(best_position2, best_node2)
            free_nodes.remove(best_node2)
      
    return np.array(cycle1), np.array(cycle2)

def ILS2(distance_matrix, data, time_MSLS, local=False):
    best_cycle_1, best_cycle_2 = gen_random_cycles(get_nodes(data))
    if local:
        best_cycle_1, best_cycle_2 = local_search_steepest(best_cycle_1, best_cycle_2, distance_matrix, data)
    best_length = calculate_cycles_length(best_cycle_1, best_cycle_2, distance_matrix)

    time_elapsed = 0

    while time_elapsed <= time_MSLS:
        start_time = time.time()
        cycle1, cycle2 = severe_perturbation(best_cycle_1, best_cycle_2, distance_matrix)
        if local:
            cycle1, cycle2 = local_search_steepest(cycle1, cycle2, distance_matrix, data)
        length = calculate_cycles_length(cycle1, cycle2, distance_matrix)
        if length < best_length:
            best_length = length
            best_cycle_1 = cycle1
            best_cycle_2 = cycle2
        stop_time = time.time()
        time_elapsed += (stop_time - start_time)
    
    return best_cycle_1, best_cycle_2

def ILS2a(distance_matrix, data, time_MSLS):
    return ILS2(distance_matrix, data, time_MSLS, local=True)