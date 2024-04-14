//
// Created by Mariusz on 23.03.2024.
//

#include "local.h"

std::vector<std::pair<int, std::pair<int, int> > > local::generate_intra_route_inner(const std::vector<int>& cycle) {
    std::vector<std::pair<int, std::pair<int, int> > > moves;
    // (0, (2, 5)) -> Oznacza odwróć podciąg zaczynając od 2 i kończąc na 5
    // (1, (2, 5)) -> Oznacza zamień wierzchołki na pozycjach 2 i 5

    int cycle_size = (int) cycle.size();

    for (int i = 0; i < cycle_size-1; ++i) {
        for (int j = i+1; j < cycle_size; ++j) {
            moves.emplace_back(SWAP_NODES, std::make_pair(i, j));
        }
    }

    for (int i = 2; i <= cycle_size-2; ++i) { // dla każdego rozmiaru okna
        for (int j = 0; j < cycle_size; ++j) { // przejedź oknem nad wektorem
            moves.emplace_back(REVERSE_SUBSEQUENCE, std::make_pair(j, (j+i) % cycle_size));
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(moves.begin(), moves.end(), g);

    return moves;
}

std::vector<int> local::local_search_steepest_inner_cycle(std::vector<int> starting_cycle, std::vector<std::vector<int> > &dist_mat) {

    for (int &node: starting_cycle) {
        node--;
    }
    // Wygeneruj wszystkie możliwe wymiany wierzchołków i wymiany krawędzi
    std::vector<std::pair<int, std::pair<int, int> > > moves = generate_intra_route_inner(starting_cycle);

    std::vector<int> tmp_cycle;
    int best_index;
    int best_value;
    int subdistance_before;
    int subdistance_after;
    int delta;
    int cycle_size = (int) starting_cycle.size();

    std::pair<int, std::pair<int, int> > move;
    // Oceń losowy ruch, jeżeli jest lepszy niż stan aktualny, to go zastosuj
    while (true) {
        best_index = -1;
        best_value = 0;

        for (int i = 0; i < moves.size(); ++i) { // Wybieranie aktualnie najlepszego ruchu

            move = moves[i];
            int node1 = move.second.first;
            int node2 = move.second.second;
            int type = move.first;

            if (type == REVERSE_SUBSEQUENCE) { // odwracanie podciągu, czyli zamiana krawędzi
                tmp_cycle = Utils::reverse_cycle(starting_cycle, node1, node2);

                int node1a = (node1 - 1 + cycle_size) % cycle_size;
                int node2a = (node2 + 1 + cycle_size) % cycle_size;

                subdistance_before =
                        dist_mat[starting_cycle[node1a]][starting_cycle[node1]]
                        + dist_mat[starting_cycle[node2]][starting_cycle[node2a]];

                subdistance_after =
                        dist_mat[tmp_cycle[node1a]][tmp_cycle[node1]]
                        + dist_mat[tmp_cycle[node2]][tmp_cycle[node2a]];

                delta = subdistance_before - subdistance_after;

                if (delta > best_value) {
                    best_value = delta;
                    best_index = i;
                }

            }
            if (type == SWAP_NODES) { // zamiana wierzchołków
                tmp_cycle = Utils::swap_nodes(starting_cycle, node1, node2);

                int node1a = (node1 - 1 + cycle_size) % cycle_size;
                int node1b = (node1 + 1 + cycle_size) % cycle_size;

                int node2a = (node2 - 1 + cycle_size) % cycle_size;
                int node2b = (node2 + 1 + cycle_size) % cycle_size;

                subdistance_before =
                        dist_mat[starting_cycle[node1]][starting_cycle[node1a]]
                        + dist_mat[starting_cycle[node1]][starting_cycle[node1b]]
                        + dist_mat[starting_cycle[node2]][starting_cycle[node2a]]
                        + dist_mat[starting_cycle[node2]][starting_cycle[node2b]];

                subdistance_after =
                        dist_mat[tmp_cycle[node1]][tmp_cycle[node1a]]
                        + dist_mat[tmp_cycle[node1]][tmp_cycle[node1b]]
                        + dist_mat[tmp_cycle[node2]][tmp_cycle[node2a]]
                        + dist_mat[tmp_cycle[node2]][tmp_cycle[node2b]];

                delta = subdistance_before - subdistance_after;

                if (delta > best_value) {
                    best_value = delta;
                    best_index = i;
                }
            }
        }

        if (best_value > 0) {
            move = moves[best_index];
            int type = move.first;
            int node1 = move.second.first;
            int node2 = move.second.second;

            if (type == REVERSE_SUBSEQUENCE) {
                starting_cycle = Utils::reverse_cycle(starting_cycle, node1, node2);
            }
            if (type == SWAP_NODES) {
                starting_cycle = Utils::swap_nodes(starting_cycle, node1, node2);
            }
        } else {
            break;
        }

    }

    for (int &i: starting_cycle) {
        i++;
    }
    return starting_cycle;

}

std::pair<std::vector<int>, std::vector<int>> local::local_search_steepest_inner(std::vector<int> &cycle_1, std::vector<int> &cycle_2, std::vector<std::vector<int> > &dist_mat) {
    std::vector<int> new_cycle_1 = local_search_steepest_inner_cycle(cycle_1, dist_mat);
    std::vector<int> new_cycle_2 = local_search_steepest_inner_cycle(cycle_2, dist_mat);

    return std::make_pair(new_cycle_1, new_cycle_2);
}

std::vector<int> local::local_search_greedy_inner_cycle(std::vector<int> starting_cycle, std::vector<std::vector<int> > &dist_mat) {

    for (int &node: starting_cycle) {
        node--;
    }
    // Wygeneruj wszystkie możliwe wymiany wierzchołków i wymiany krawędzi
    std::vector<std::pair<int, std::pair<int, int> > > moves = generate_intra_route_inner(starting_cycle);

    std::vector<int> tmp_cycle;
    int best_index;
    int best_value;
    int subdistance_before;
    int subdistance_after;
    int delta;
    int cycle_size = (int) starting_cycle.size();

    std::pair<int, std::pair<int, int> > move;
    // Oceń losowy ruch, jeżeli jest lepszy niż stan aktualny, to go zastosuj
    while (true) {

        best_index = -1;
        best_value = 0;

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(moves.begin(), moves.end(), g);

        for (int i = 0; i < moves.size(); ++i) { // Wybieranie aktualnie najlepszego ruchu

            move = moves[i];
            int node1 = move.second.first;
            int node2 = move.second.second;
            int type = move.first;

            if (type == REVERSE_SUBSEQUENCE) { // odwracanie podciągu, czyli zamiana krawędzi
                tmp_cycle = Utils::reverse_cycle(starting_cycle, node1, node2);

                int node1a = (node1 - 1 + cycle_size) % cycle_size;
                int node2a = (node2 + 1 + cycle_size) % cycle_size;

                subdistance_before =
                        dist_mat[starting_cycle[node1a]][starting_cycle[node1]]
                        + dist_mat[starting_cycle[node2]][starting_cycle[node2a]];

                subdistance_after =
                        dist_mat[tmp_cycle[node1a]][tmp_cycle[node1]]
                        + dist_mat[tmp_cycle[node2]][tmp_cycle[node2a]];

                delta = subdistance_before - subdistance_after;

                if (delta > best_value) {
                    best_value = delta;
                    best_index = i;
                    break;
                }

            }
            if (type == SWAP_NODES) { // zamiana wierzchołków
                tmp_cycle = Utils::swap_nodes(starting_cycle, node1, node2);

                int node1a = (node1 - 1 + cycle_size) % cycle_size;
                int node1b = (node1 + 1 + cycle_size) % cycle_size;

                int node2a = (node2 - 1 + cycle_size) % cycle_size;
                int node2b = (node2 + 1 + cycle_size) % cycle_size;

                subdistance_before =
                        dist_mat[starting_cycle[node1]][starting_cycle[node1a]]
                        + dist_mat[starting_cycle[node1]][starting_cycle[node1b]]
                        + dist_mat[starting_cycle[node2]][starting_cycle[node2a]]
                        + dist_mat[starting_cycle[node2]][starting_cycle[node2b]];

                subdistance_after =
                        dist_mat[tmp_cycle[node1]][tmp_cycle[node1a]]
                        + dist_mat[tmp_cycle[node1]][tmp_cycle[node1b]]
                        + dist_mat[tmp_cycle[node2]][tmp_cycle[node2a]]
                        + dist_mat[tmp_cycle[node2]][tmp_cycle[node2b]];

                delta = subdistance_before - subdistance_after;

                if (delta > best_value) {
                    best_value = delta;
                    best_index = i;
                    break;
                }
            }
        }

        if (best_value > 0) {
            move = moves[best_index];
            int type = move.first;
            int node1 = move.second.first;
            int node2 = move.second.second;

            if (type == REVERSE_SUBSEQUENCE) {
                starting_cycle = Utils::reverse_cycle(starting_cycle, node1, node2);
            }
            if (type == SWAP_NODES) {
                starting_cycle = Utils::swap_nodes(starting_cycle, node1, node2);
            }
        } else {
            break;
        }

    }

    for (int &i: starting_cycle) {
        i++;
    }
    return starting_cycle;

}

std::pair<std::vector<int>, std::vector<int>> local::local_search_greedy_inner(std::vector<int> &cycle_1, std::vector<int> &cycle_2, std::vector<std::vector<int> > &dist_mat) {
    std::vector<int> new_cycle_1 = local_search_greedy_inner_cycle(cycle_1, dist_mat);
    std::vector<int> new_cycle_2 = local_search_greedy_inner_cycle(cycle_2, dist_mat);

    return std::make_pair(new_cycle_1, new_cycle_2);
}

std::pair<std::vector<int>, std::vector<int> > local::swap_nodes_between(std::vector<int> cycle1, std::vector<int> cycle2, int pos_a, int pos_b) {

    std::vector<int> tmp_cycle1 = cycle1;
    std::vector<int> tmp_cycle2 = cycle2;

    int tmp = tmp_cycle1[pos_a];
    tmp_cycle1[pos_a] = tmp_cycle2[pos_b];
    tmp_cycle2[pos_b] = tmp;

    return std::make_pair(tmp_cycle1, tmp_cycle2);
}

std::pair<std::vector<int>, std::vector<int>> local::local_search_steepest_between(std::vector<int> cycle_1, std::vector<int> cycle_2, std::vector<std::vector<int> > &dist_mat) {

    for (int &node: cycle_1) {
        node--;
    }

    for (int &node: cycle_2) {
        node--;
    }
    // Wygeneruj wszystkie możliwe wymiany wierzchołków pomiędzy cyklami
    std::vector<std::pair<int, int> > moves;
    for (int i = 0; i < cycle_1.size(); ++i) {
        for (int j = 0; j < cycle_2.size(); ++j) {
            moves.emplace_back(i, j);
        }
    }

    std::vector<int> tmp_cycle_1;
    std::vector<int> tmp_cycle_2;

    int best_index;
    int best_value;
    int subdistance_before;
    int subdistance_after;
    int delta;

    int cycle_1_size = (int) cycle_1.size();
    int cycle_2_size = (int) cycle_2.size();


    std::pair<int, int> move;
    // Oceń losowy ruch, jeżeli jest lepszy niż stan aktualny, to go zastosuj
    while (true) {
        best_index = -1;
        best_value = 0;

        for (int i = 0; i < moves.size(); ++i) { // Wybieranie aktualnie najlepszego ruchu

            move = moves[i];
            int node1 = move.first;
            int node2 = move.second;

            std::pair<std::vector<int>, std::vector<int>> cycles = swap_nodes_between(cycle_1, cycle_2, node1, node2);

            tmp_cycle_1 = cycles.first;
            tmp_cycle_2 = cycles.second;

            int node1a = (node1 - 1 + cycle_1_size) % cycle_1_size;
            int node1b = (node1 + 1 + cycle_1_size) % cycle_1_size;

            int node2a = (node2 - 1 + cycle_2_size) % cycle_2_size;
            int node2b = (node2 + 1 + cycle_2_size) % cycle_2_size;

            subdistance_before =
                    dist_mat[cycle_1[node1]][cycle_1[node1a]]
                    +   dist_mat[cycle_1[node1]][cycle_1[node1b]]
                    +   dist_mat[cycle_2[node2]][cycle_2[node2a]]
                    +   dist_mat[cycle_2[node2]][cycle_2[node2b]];

            subdistance_after =
                    dist_mat[tmp_cycle_1[node1]][tmp_cycle_1[node1a]]
                    +   dist_mat[tmp_cycle_1[node1]][tmp_cycle_1[node1b]]
                    +   dist_mat[tmp_cycle_2[node2]][tmp_cycle_2[node2a]]
                    +   dist_mat[tmp_cycle_2[node2]][tmp_cycle_2[node2b]];

            delta = subdistance_before - subdistance_after;

            if (delta > best_value) {
                best_value = delta;
                best_index = i;
            }

        }

        if (best_value > 0) {
            move = moves[best_index];
            int node1 = move.first;
            int node2 = move.second;
            std::pair<std::vector<int>, std::vector<int>> cycles = swap_nodes_between(cycle_1, cycle_2, node1, node2);
            cycle_1 = cycles.first;
            cycle_2 = cycles.second;
        } else {
            break;
        }

    }

    for (int &i: cycle_1) {
        i++;
    }

    for (int &i: cycle_2) {
        i++;
    }
    return std::make_pair(cycle_1, cycle_2);

}

std::pair<std::vector<int>, std::vector<int>> local::local_search_greedy_between(std::vector<int> cycle_1, std::vector<int> cycle_2, std::vector<std::vector<int> > &dist_mat) {

    for (int &node: cycle_1) {
        node--;
    }

    for (int &node: cycle_2) {
        node--;
    }
    // Wygeneruj wszystkie możliwe wymiany wierzchołków pomiędzy cyklami
    std::vector<std::pair<int, int> > moves;
    for (int i = 0; i < cycle_1.size(); ++i) {
        for (int j = 0; j < cycle_2.size(); ++j) {
            moves.emplace_back(i, j);
        }
    }

    std::vector<int> tmp_cycle_1;
    std::vector<int> tmp_cycle_2;

    int best_index;
    int best_value;
    int subdistance_before;
    int subdistance_after;
    int delta;

    int cycle_1_size = (int) cycle_1.size();
    int cycle_2_size = (int) cycle_2.size();


    std::pair<int, int> move;
    // Oceń losowy ruch, jeżeli jest lepszy niż stan aktualny, to go zastosuj
    while (true) {
        best_index = -1;
        best_value = 0;

        for (int i = 0; i < moves.size(); ++i) { // Wybieranie aktualnie najlepszego ruchu

            move = moves[i];
            int node1 = move.first;
            int node2 = move.second;

            std::pair<std::vector<int>, std::vector<int>> cycles = swap_nodes_between(cycle_1, cycle_2, node1, node2);

            tmp_cycle_1 = cycles.first;
            tmp_cycle_2 = cycles.second;

            int node1a = (node1 - 1 + cycle_1_size) % cycle_1_size;
            int node1b = (node1 + 1 + cycle_1_size) % cycle_1_size;

            int node2a = (node2 - 1 + cycle_2_size) % cycle_2_size;
            int node2b = (node2 + 1 + cycle_2_size) % cycle_2_size;

            subdistance_before =
                    dist_mat[cycle_1[node1]][cycle_1[node1a]]
                    +   dist_mat[cycle_1[node1]][cycle_1[node1b]]
                    +   dist_mat[cycle_2[node2]][cycle_2[node2a]]
                    +   dist_mat[cycle_2[node2]][cycle_2[node2b]];

            subdistance_after =
                    dist_mat[tmp_cycle_1[node1]][tmp_cycle_1[node1a]]
                    +   dist_mat[tmp_cycle_1[node1]][tmp_cycle_1[node1b]]
                    +   dist_mat[tmp_cycle_2[node2]][tmp_cycle_2[node2a]]
                    +   dist_mat[tmp_cycle_2[node2]][tmp_cycle_2[node2b]];

            delta = subdistance_before - subdistance_after;

            if (delta > best_value) {
                best_value = delta;
                best_index = i;
                break;
            }

        }

        if (best_value > 0) {
            move = moves[best_index];
            int node1 = move.first;
            int node2 = move.second;
            std::pair<std::vector<int>, std::vector<int>> cycles = swap_nodes_between(cycle_1, cycle_2, node1, node2);
            cycle_1 = cycles.first;
            cycle_2 = cycles.second;
        } else {
            break;
        }

    }

    for (int &i: cycle_1) {
        i++;
    }

    for (int &i: cycle_2) {
        i++;
    }
    return std::make_pair(cycle_1, cycle_2);

}

std::vector<int> local::random_search_cycle(std::vector<int> &starting_cycle, std::vector<std::vector<int> > &dist_mat, std::chrono::microseconds time_limit_microseconds) {

    for (int &node: starting_cycle) {
        node--;
    }

    std::vector<std::pair<int, std::pair<int, int> > > moves = generate_intra_route_inner(starting_cycle);
    std::pair<int, std::pair<int, int> > move;

    auto start = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds delta;

    do {

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(moves.begin(), moves.end(), g);
        move = moves[0];

        int type = move.first;
        int node1 = move.second.first;
        int node2 = move.second.second;

        if (type == REVERSE_SUBSEQUENCE) {
            starting_cycle = Utils::reverse_cycle(starting_cycle, node1, node2);
        }
        if (type == SWAP_NODES) {
            starting_cycle = Utils::swap_nodes(starting_cycle, node1, node2);
        }

        auto current = std::chrono::high_resolution_clock::now();
        delta = std::chrono::duration_cast<std::chrono::microseconds>(current - start);

    } while (delta < time_limit_microseconds);

    for (int &i: starting_cycle) {
        i++;
    }
    return starting_cycle;
}

std::pair<std::vector<int>, std::vector<int>> local::random_search(std::vector<int> &cycle_1, std::vector<int> &cycle_2, std::vector<std::vector<int> > &dist_mat, int time_limit) {

    std::vector<int> new_cycle_1 = random_search_cycle(cycle_1, dist_mat, std::chrono::microseconds(time_limit / 2));
    std::vector<int> new_cycle_2 = random_search_cycle(cycle_2, dist_mat, std::chrono::microseconds(time_limit / 2));

    return std::make_pair(new_cycle_1, new_cycle_2);
}

std::pair<std::vector<int>, std::vector<int>> local::candidate_moves_algorithm(std::vector<int> cycle_1, std::vector<int> cycle_2, std::vector<std::vector<int> > &dist_mat, int k) {
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
                    std::pair<std::vector<int>, std::vector<int>> cycles = swap_nodes_between(cycle_1, cycle_2, node_1_search.second, node_2_search.second);

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
            std::pair<std::vector<int>, std::vector<int>> cycles = swap_nodes_between(cycle_1, cycle_2, node1, node2);
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