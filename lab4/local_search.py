from moves import *
from utils import *

def rank_move(move, cycle1, cycle2, distance):
    subdistance_before = 0
    subdistance_after = 0

    move_type, (A, B) = move

    if move_type == SWAP_NODES_BETWEEN:
        a = cycle1[A]
        b = cycle2[B]

        a_prev = cycle1[(A-1) % len(cycle1)]
        a_next = cycle1[(A+1) % len(cycle1)]

        b_prev = cycle2[(B-1) % len(cycle2)]
        b_next = cycle2[(B+1) % len(cycle2)]

        subdistance_before += distance[a_prev][a] + distance[a][a_next] + distance[b_prev][b] + distance[b][b_next]
        subdistance_after += distance[a_prev][b] + distance[b][a_next] + distance[b_prev][a] + distance[a][b_next]

    elif move_type == SWAP_EDGES_WITHIN_CYCLE_1:
        a = cycle1[A]
        b = cycle1[B]

        a_prev = cycle1[(A-1) % len(cycle1)]
        b_next = cycle1[(B+1) % len(cycle1)]

        subdistance_before += distance[a_prev][a] + distance[b][b_next]
        subdistance_after += distance[a_prev][b] + distance[a][b_next]

    elif move_type == SWAP_EDGES_WITHIN_CYCLE_2:

        a = cycle2[A]
        b = cycle2[B]

        a_prev = cycle2[(A-1) % len(cycle2)]
        b_next = cycle2[(B+1) % len(cycle2)]

        subdistance_before += distance[a_prev][a] + distance[b][b_next]
        subdistance_after += distance[a_prev][b] + distance[a][b_next]

    return subdistance_after - subdistance_before

def local_search_steepest(cycle1, cycle2, distance, data, VISUALIZE=False):

    moves = generate_moves(cycle1)

    while True:

        best_move = None
        best_delta = 0

        for move in moves:

            delta = rank_move(move, cycle1, cycle2, distance)
            if delta < best_delta:
                best_move = move
                best_delta = delta
                
        if best_delta < 0:
            cycle1, cycle2 = apply_move(best_move, cycle1, cycle2)
            if VISUALIZE:
                clear_output(wait=True)
                plot_cycles(cycle1, cycle2, data)
        elif best_move is None:
            break

    return cycle1, cycle2
