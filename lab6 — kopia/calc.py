import numpy as np
from utils import calculate_cycles_length

def cycle_to_edges(cycle):
    edges = []
    for i in range(len(cycle)):
        edges.append((cycle[i], cycle[(i+1) % len(cycle)]))
    return edges

def calculate_common_edges(c1a, c1b, c2a, c2b):
    edges_in_c1 = set(cycle_to_edges(c1a)).union(cycle_to_edges(c1b))
    edges_in_c2 = set(cycle_to_edges(c2a)).union(cycle_to_edges(c2b))

    common_edges = edges_in_c1.intersection(edges_in_c2)
    percentage = len(common_edges) / len(edges_in_c1)
    return percentage

def calculate_common_nodes(c1a, c1b, c2a, c2b):
    c1a_set, c1b_set = set(c1a), set(c1b)
    c2a_set, c2b_set = set(c2a), set(c2b)

    common_nodes_1 = len(c1a_set.intersection(c2a_set)) + len(c1b_set.intersection(c2b_set))
    common_nodes_2 = len(c1a_set.intersection(c2b_set)) + len(c1b_set.intersection(c2a_set))

    max_nodes = max(common_nodes_1, common_nodes_2)
    percentage = max_nodes / (len(c1a) + len(c1b))
    return percentage

def calculate_mean_common_edges(c1a, c1b, cycles):
    common_edges = (calculate_common_edges(c1a, c1b, c2a, c2b) for c2a, c2b in cycles)
    return np.mean(list(common_edges))

def calculate_mean_common_nodes(c1a, c1b, cycles):
    common_nodes = (calculate_common_nodes(c1a, c1b, c2a, c2b) for c2a, c2b in cycles)
    return np.mean(list(common_nodes))

def calculate_metrics(cycle_pair, best_cycles, cycles, dist_matrix):
    length = calculate_cycles_length(*cycle_pair, dist_matrix)
    e = calculate_common_edges(*cycle_pair, *best_cycles)
    n = calculate_common_nodes(*cycle_pair, *best_cycles)
    e_mean = calculate_mean_common_edges(*cycle_pair, cycles)
    n_mean = calculate_mean_common_nodes(*cycle_pair, cycles)

    return {
        "length": length,
        "common_edges": e,
        "common_nodes": n,
        "common_edges_mean": e_mean,
        "common_nodes_mean": n_mean
    }
