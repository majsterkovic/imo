//
// Created by Mariusz on 14.04.2024.
//

#include "local_optim.h"
#include "local.h"


std::pair<std::vector<int>, std::vector<int>>
local_optim::candidate_moves_algorithm(std::vector<int> cycle_1, std::vector<int> cycle_2, std::vector<std::vector<int> > &dist_mat, int k) {
    for (int &node: cycle_1) {
        node--;
    }

    for (int &node: cycle_2) {
        node--;
    }
    int num_cities = dist_mat[0].size();

    std::vector<std::vector<std::pair<int, int>>> closest = Utils::compute_k_closest_neighbours(dist_mat, k);

    int best_delta;
    bool found_best_move;

    while (true) {
        std::pair<int, std::pair<std::pair<int, int>, std::pair<int, int>>> best_move;
        found_best_move = false;
        best_delta = 0;

        for (int i = 0; i < num_cities; ++i) {
            for (int j = 0; j < k; ++j) {
                if (i == j) {
                    continue;
                }
                std::vector<int> starting_cycle;
                std::pair<int, int> node_1_search = Utils::find_city(cycle_1, cycle_2, i);
                std::pair<int, int> node_2_search = Utils::find_city(cycle_1, cycle_2, j);
                std::pair<int, std::pair<std::pair<int, int>, std::pair<int, int>>> move;
                std::vector<int> tmp_cycle;
                std::vector<int> tmp_cycle_1;
                std::vector<int> tmp_cycle_2;
                int delta = 0;

                if (node_1_search.first == node_2_search.first) {

                    if (node_1_search.first == 1) {
                        starting_cycle = cycle_1;
                    } else if (node_1_search.first == 2) {
                        starting_cycle = cycle_2;
                    }
                    int cycle_size = (int) starting_cycle.size();
                    tmp_cycle = Utils::reverse_cycle(starting_cycle, node_1_search.second, node_2_search.second);

                    int node1a = (node_1_search.second - 1 + cycle_size) % cycle_size;
                    int node2a = (node_2_search.second + 1 + cycle_size) % cycle_size;

                    int subdistance_before =
                            dist_mat[starting_cycle[node1a]][starting_cycle[node_1_search.second]]
                            + dist_mat[starting_cycle[node_2_search.second]][starting_cycle[node2a]];

                    int subdistance_after =
                            dist_mat[tmp_cycle[node1a]][tmp_cycle[node_1_search.second]]
                            + dist_mat[tmp_cycle[node_2_search.second]][tmp_cycle[node2a]];

                    delta = subdistance_before - subdistance_after;
                    move = std::make_pair(REVERSE_SUBSEQUENCE, std::make_pair(std::make_pair(node_1_search.first, node_2_search.first), std::make_pair(node_1_search.second, node_2_search.second)));
                } else {
                    std::pair<std::vector<int>, std::vector<int>> cycles = local::swap_nodes_between(cycle_1, cycle_2, node_1_search.second, node_2_search.second);

                    tmp_cycle_1 = cycles.first;
                    tmp_cycle_2 = cycles.second;

                    int cycle_1_size = cycle_1.size();
                    int cycle_2_size = cycle_2.size();

                    int node1a = (node_1_search.second - 1 + cycle_1_size) % cycle_1_size;
                    int node1b = (node_1_search.second + 1 + cycle_1_size) % cycle_1_size;

                    int node2a = (node_2_search.second - 1 + cycle_2_size) % cycle_2_size;
                    int node2b = (node_2_search.second + 1 + cycle_2_size) % cycle_2_size;

                    int subdistance_before =
                            dist_mat[cycle_1[node_1_search.second]][cycle_1[node1a]]
                            +   dist_mat[cycle_1[node_1_search.second]][cycle_1[node1b]]
                            +   dist_mat[cycle_2[node_2_search.second]][cycle_2[node2a]]
                            +   dist_mat[cycle_2[node_2_search.second]][cycle_2[node2b]];

                    int subdistance_after =
                            dist_mat[tmp_cycle_1[node_1_search.second]][tmp_cycle_1[node1a]]
                            +   dist_mat[tmp_cycle_1[node_1_search.second]][tmp_cycle_1[node1b]]
                            +   dist_mat[tmp_cycle_2[node_2_search.second]][tmp_cycle_2[node2a]]
                            +   dist_mat[tmp_cycle_2[node_2_search.second]][tmp_cycle_2[node2b]];

                    delta = subdistance_before - subdistance_after;
                    move = std::make_pair(SWAP_NODES, std::make_pair(std::make_pair(node_1_search.first, node_2_search.first), std::make_pair(node_1_search.second, node_2_search.second)));
                }

                if (best_delta < delta) {
                    best_delta = delta;
                    found_best_move = true;
                    best_move = move;
                }
            }
        }
        if (!found_best_move) {
            break;
        }

        if (best_move.first == REVERSE_SUBSEQUENCE) {
            if (best_move.second.first.first == 1) {
                cycle_1 = Utils::reverse_cycle(cycle_1, best_move.second.second.first, best_move.second.second.second);
            } else if (best_move.second.first.first == 2) {
                cycle_2 = Utils::reverse_cycle(cycle_2, best_move.second.second.first, best_move.second.second.second);
            }
        } else if (best_move.first == SWAP_NODES) {
            int node1 = best_move.second.second.first;
            int node2 = best_move.second.second.second;
            std::pair<std::vector<int>, std::vector<int>> cycles = local::swap_nodes_between(cycle_1, cycle_2, node1, node2);
            cycle_1 = cycles.first;
            cycle_2 = cycles.second;
        }
    }
    for (int &node: cycle_1) {
        node++;
    }

    for (int &node: cycle_2) {
        node++;
    }
    return std::make_pair(cycle_1, cycle_2);
}



std::vector<local_optim::move>
local_optim::generate_init_moves(const std::vector<int> &cycle1, const std::vector<int> &cycle2, std::vector<std::vector<int> > &dist_mat) {
    std::vector<local_optim::move> moves;
    int subdistance_before, subdistance_after, delta;
    int cycle_1_size = (int) cycle1.size();
    int cycle_2_size = (int) cycle2.size();

    // wymiany wierzchołków między cyklami
    for (int i = 0; i < cycle_1_size; i++){
        for (int j = 0; j < cycle_2_size; j++) {

            int node_b = cycle1[i];
            int node_e = cycle2[j];

            int node_a = cycle1[(i - 1 + cycle_1_size) % cycle_1_size];
            int node_c = cycle1[(i + 1 + cycle_1_size) % cycle_1_size];

            int node_d = cycle2[(j - 1 + cycle_2_size) % cycle_2_size];
            int node_f = cycle2[(j + 1 + cycle_2_size) % cycle_2_size];

            std::vector<int> data = {node_a, node_b, node_c, node_d, node_e, node_f};
            moves.emplace_back(SWAP_NODES_BETWEEN_CYCLES, data, 0);

            subdistance_before =
                    dist_mat[node_a][node_b]
                    + dist_mat[node_b][node_c]
                    + dist_mat[node_d][node_e]
                    + dist_mat[node_e][node_f];


            subdistance_after =
                    dist_mat[node_a][node_e]
                    + dist_mat[node_e][node_c]
                    + dist_mat[node_d][node_b]
                    + dist_mat[node_b][node_f];

            delta = subdistance_before - subdistance_after;

            if (delta > 0) {
                moves.back().score = delta;
            } else {
                moves.pop_back();
            }
        }

    }

    // wymiany krawędzi wewnątrz cykli
    std::vector<std::vector<int> > cycles = {cycle1, cycle2};
    // iterate over cycles
    for (const std::vector<int> &cycle: cycles) {
        int cycle_size = (int) cycle.size();

        for (int i = 0; i < cycle_size; i++) {
            for (int j = 0; j < cycle_size; j++) {
                if (i == j) { continue; }

                int node_a = cycle[(i - 1 + cycle_size) % cycle_size];
                int node_b = cycle[i];
                int node_c = cycle[j];
                int node_d = cycle[(j + 1 + cycle_size) % cycle_size];

                std::vector<int> data = {node_a, node_b, node_c, node_d};
                moves.emplace_back(SWAP_EDGE, data, 0);

                subdistance_before =
                        dist_mat[node_b][node_a]
                        + dist_mat[node_c][node_d];

                subdistance_after =
                        dist_mat[node_b][node_d]
                        + dist_mat[node_c][node_a];

                delta = subdistance_before - subdistance_after;

                if (delta > 0) {
                    moves.back().score = delta;
                } else {
                    moves.pop_back();
                }

            }

        }
    }

    std::sort(moves.begin(), moves.end(), [](const move &a, const move &b) { return a.score > b.score; });
    return moves;
}

void print_move(local_optim::move m) {
    std::cout << "Type: " << m.type << std::endl;
    std::cout << "Data: ";
    for (int i = 0; i < m.data.size(); i++) {
        std::cout << m.data[i] << " ";
    }
    std::cout << std::endl;
}

bool has_edges(std::vector<int> cycle, std::vector<int> edges) {
    int n = edges.size();
    bool found_first = false;
    bool found_second = false;
    if (n == 4) {
        // search for nodes a-b and c-d in cycle
        for (int i = 0; i < cycle.size(); i++) {
            if (cycle[i] == edges[0]) {
                if (cycle[(i + 1) % cycle.size()] == edges[1]) {
                    found_first = true;
                    break;
                }
            }
        }
        for (int i = 0; i < cycle.size(); i++) {
            if (cycle[i] == edges[2]) {
                if (cycle[(i + 1) % cycle.size()] == edges[3]) {
                    found_second = true;
                    break;
                }
            }
        }
        return found_first && found_second;
    }
    if (n == 3) {
        // search for nodes a-b-c
        for (int i = 0; i < cycle.size(); i++) {
            if (cycle[i] == edges[0]) {
                if (cycle[(i + 1) % cycle.size()] == edges[1]) {
                    if (cycle[(i + 2) % cycle.size()] == edges[2]) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void remove_moves(std::vector<local_optim::move>& moves, const std::vector<local_optim::move>& to_delete) {
    for (const auto& move_to_delete : to_delete) {
        moves.erase(std::remove(moves.begin(), moves.end(), move_to_delete), moves.end());
    }
}

std::pair<std::vector<int>, std::vector<int> > apply_move(const local_optim::move& best_move, std::vector<int>& cycle_1, std::vector<int>& cycle_2, int cycle_index) {
    switch (best_move.type) {
        case SWAP_NODES_BETWEEN_CYCLES: {
            // Zamiana wierzchołków między cyklami
            int node1_value = best_move.data[1];
            int node2_value = best_move.data[4];

            auto it_node1 = std::find(cycle_1.begin(), cycle_1.end(), node1_value);
            auto it_node2 = std::find(cycle_2.begin(), cycle_2.end(), node2_value);

            std::pair<std::vector<int>, std::vector<int> > cycles = local::swap_nodes_between(cycle_1, cycle_2, it_node1 - cycle_1.begin(), it_node2 - cycle_2.begin());
            return cycles;
        }
        case SWAP_EDGE: {
            // Zamiana krawędzi wewnątrz cyklu

            std::vector edge1 = {best_move.data[0], best_move.data[1]};
            std::vector edge2 = {best_move.data[2], best_move.data[3]};

            auto it_node1 = std::find(cycle_1.begin(), cycle_1.end(), edge1[1]);
            auto it_node2 = std::find(cycle_1.begin(), cycle_1.end(), edge2[0]);

            if (cycle_index == 1) {
                std::vector cycle = Utils::reverse_cycle(cycle_1, it_node1 - cycle_1.begin(), it_node2 - cycle_1.begin());
                return std::make_pair(cycle, cycle_2);
            }
            else {
                std::vector cycle = Utils::reverse_cycle(cycle_2, it_node1 - cycle_2.begin(), it_node2 - cycle_2.begin());
                return std::make_pair(cycle_1, cycle);
            }

        }
            // Dodaj tutaj inne typy ruchów, jeśli są używane
        default:
            // Nieznany typ ruchu
            break;
    }
}

std::pair<std::vector<int>, std::vector<int>>
local_optim::cache_algorithm(std::vector<int> cycle_1, std::vector<int> cycle_2, std::vector<std::vector<int>> &dist_mat) {

    for (int &node: cycle_1) { node--; }

    for (int &node: cycle_2) { node--; }

    std::vector<move> moves = generate_init_moves(cycle_1, cycle_2, dist_mat);

        while(true) {

            std::vector<move> to_delete;
            move best_move;
            int cycle_index = 0;
            best_move.score = 0;

            for (move &m: moves) {
                int type = m.type;
                //print_move(m);
                if (type == SWAP_EDGE) {
                    std::vector<int> edges = m.data;
                    std::vector<int> reversed_edges = {edges[1], edges[0], edges[3], edges[2]};

                    if (has_edges(cycle_1, edges)) {
                        to_delete.push_back(m);
                        best_move = m;
                        cycle_index = 1;
                    }

                    else if (has_edges(cycle_2, edges)) {
                        to_delete.push_back(m);
                        best_move = m;
                        cycle_index = 2;
                    }


                    if (has_edges(cycle_1, reversed_edges) )
                    {
                        to_delete.push_back(m);
                        best_move = m;
                        cycle_index = 1;
                    }
                    else if (has_edges(cycle_2, reversed_edges)) {
                        to_delete.push_back(m);
                        best_move = m;
                        cycle_index = 2;
                    }

                    else {
                        to_delete.push_back(m);
                        break;
                    }
                }
                else if (type == SWAP_NODES_BETWEEN_CYCLES) {
                    std::vector<int> edges = m.data;
                    std::vector<int> first_cycle_edges = {edges[0], edges[1], edges[2]};
                    std::vector<int> second_cycle_edges = {edges[3], edges[4], edges[5]};

                    if (has_edges(cycle_1, first_cycle_edges) && has_edges(cycle_2, second_cycle_edges)) {
                        to_delete.push_back(m);
                        best_move = m;
                    } else {
                        to_delete.push_back(m);
                        break;
                    }
                }
            }

            if (best_move.score == 0) {
                break;
            }

            remove_moves(moves, to_delete);
            std::pair new_cycles = apply_move(best_move, cycle_1, cycle_2, cycle_index);

            // add new moves...

            cycle_1 = new_cycles.first;
            cycle_2 = new_cycles.second;


    }


    Utils::incrementVectors(cycle_1, cycle_2);
    return std::make_pair(cycle_1, cycle_2);
}





