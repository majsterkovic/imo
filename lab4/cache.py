from utils import *
from moves import *

def generate_cache_moves(cycle, move_type):
    edges = []
    combinations = []
    for i in range(len(cycle)):
        j = (i + 1) % len(cycle)
        edges.append(((cycle[i], cycle[j]), (i, j)))

    for i in range(len(edges)):
        for j in range(len(edges)):
            if i == j:
                continue
            (a, b), coords_0 = edges[i]
            (c, d), coords_1 = edges[j]
            if a == c or a == d or b == c or b == d:
                continue
            move = (move_type, (coords_0[1], coords_1[0]))
            combinations.append(((a,b), (c,d), move))

    return combinations

def rank_move_coords(move_with_coords, distance):
    (a, b), (c, d), move = move_with_coords
    distance_before = distance[a][b] + distance[c][d]
    distance_after = distance[a][c] + distance[b][d]
    rank = distance_after - distance_before
    return (a, b), (c, d), move, rank

def rank_moves(moves_with_coords, distance):
    moves_with_coords = [move for move in (rank_move_coords(move_with_coords, distance) for move_with_coords in moves_with_coords) if move[3] < 0]
    moves_with_coords.sort(key=lambda x: x[3])
    return moves_with_coords

def generate_all_cache_moves(cycle1, cycle2, distance):
    moves = []
    moves.extend(generate_cache_moves(cycle1, SWAP_EDGES_WITHIN_CYCLE_1))
    moves.extend(generate_cache_moves(cycle2, SWAP_EDGES_WITHIN_CYCLE_2))
    return rank_moves(moves, distance)

def find_edge_in_cycle(edge, cycle):
    for i in range(len(cycle)):
        j = (i+1) % len(cycle)
        if cycle[i] == edge[0] and cycle[j] == edge[1]:
            return (i, j)
    return (None, None)

def find_edges_in_cycle(edge1, edge2, cycle):
    e1_coords = find_edge_in_cycle(edge1, cycle)
    if e1_coords[0] is not None:
        e2_coords = find_edge_in_cycle(edge2, cycle)
        if e2_coords[0] is not None:
            return (e1_coords[1], e2_coords[0])
    return None

def remove_marked(moves, banned_edges):
    return [m for m in moves if m[0] not in banned_edges and m[1] not in banned_edges]

def create_new_moves(cycle, allowed_edges, move_type, distance):
    edges = []
    combinations = []
    for i in range(len(cycle)):
        j = (i + 1) % (len(cycle))
        edges.append(((cycle[i], cycle[j]), (i, j)))
    
    for i in range(len(edges)):
        for j in range(len(edges)):
            print
            if i == j:
                continue
            if edges[i][0] in allowed_edges or edges[j][0] in allowed_edges:
                (a, b), coords_0 = edges[i]
                (c, d), coords_1 = edges[j]
                if a == c or a == d or b == c or b == d:
                    continue
                move = (move_type, (coords_0[1], coords_1[0]))
                combinations.append(((a,b), (c,d), move))

    ranked_combinations = rank_moves(combinations, distance)
    return ranked_combinations
    
    
def generate_all_new_moves(banned_edges, cycle1, cycle2, distance):
    moves = []
    moves.extend(create_new_moves(cycle1, banned_edges, SWAP_EDGES_WITHIN_CYCLE_1, distance))
    moves.extend(create_new_moves(cycle2, banned_edges, SWAP_EDGES_WITHIN_CYCLE_2, distance))
    moves = list(filter(lambda x: x[3] < 0, moves))
    return moves


def cache_moves_algorithm(cycle1, cycle2, distance, data, VISUALIZE=False):
    ranked_moves = generate_all_cache_moves(cycle1, cycle2, distance)
    
    while True:
  
        appliable_move = None
        banned_edges = set()
        allowed_edges = set()

        for ranked_move in ranked_moves:
            (a, b), (c, d), move, _ = ranked_move
            move_type, (_, _) = move

            edges_in_c1, edges_in_c2 = None, None

            if move_type == SWAP_EDGES_WITHIN_CYCLE_1:
                edges_in_c1 = find_edges_in_cycle((a, b), (c, d), cycle1)
            

            if move_type == SWAP_EDGES_WITHIN_CYCLE_2: 
                edges_in_c2 = find_edges_in_cycle((a, b), (c, d), cycle2)
            

            if edges_in_c1 is not None:
                # print(f"Edges in cycle 1: {a} {b} {c} {d}")
                appliable_move = (move_type, edges_in_c1)
                banned_edges.add((a, b))
                banned_edges.add((c, d))
                allowed_edges.add((a, c))
                allowed_edges.add((b, d))
                break

            if edges_in_c2 is not None:
                # print(f"Edges in cycle 2: {a} {b} {c} {d}")
                appliable_move = (move_type, edges_in_c2)
                banned_edges.add((a, b))
                banned_edges.add((c, d))
                allowed_edges.add((a, c))
                allowed_edges.add((b, d))
                break

        if appliable_move is None:
            break
        else:
            cycle1, cycle2 = apply_move(appliable_move, cycle1, cycle2)

        if VISUALIZE:
            clear_output(wait=True)
            plot_cycles(cycle2, cycle1, data)
        
        ranked_moves = remove_marked(ranked_moves, banned_edges)
        new = generate_all_new_moves(allowed_edges, cycle1, cycle2, distance)
        ranked_moves.extend(new)
        ranked_moves.sort(key=lambda x: x[3])

    return cycle1, cycle2
