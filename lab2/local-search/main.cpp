

/*
 Zadanie polega na implementacji lokalnego przeszukiwania w wersjach stromej (steepest) i
zachłannej (greedy), z dwoma różnym rodzajami sąsiedztwa, starując albo z rozwiązań losowych, albo
z rozwiązań uzyskanych za pomocą jednej z heurystyk opracowanych w ramach poprzedniego
zadania. W sumie 8 kombinacji - wersji lokalnego przeszukiwania.
Jako punkt odniesienia należy zaimplementować algorytm losowego błądzenia, który w każdej iteracji
wykonuje losowo wybrany ruch (niezależnie od jego oceny) i zwraca najlepsze znalezione w ten
sposób rozwiązanie. Algorytm ten powinien działać w takim samym czasie jak średnio najwolniejsza z
wersji lokalnego przeszukiwania.
Sąsiedztwa
W przypadku rozważanego problemu potrzebne będą dwa typy ruchów: ruchy zmieniające zbiory
wierzchołków tworzące dwa cykle i ruchy wewnątrztrasowe, które jedynie zmieniają kolejność
wierzchołków na trasie.
2 - różne starty
2 - wersje algorytmu
2 - ruchy wewnątrz trasy / zmieniające zbiory wierzchołków

Rozaje ruchów:
wymiana dwóch wierzchołków (zamiana dwóch cyfr miejscami)
wymiana dwóch krawędzi (odwrócenie podciągu o długości co najmniej 2 ale co najmniej 2 elementy powinny zostać na swoich miejscach)

1 2 3 4 5 6 7 8 9
1 2 6 5 4 3 7 8 9
 */

#include <pstl/parallel_backend_utils.h>

#include "Utils.h"
#include "greedy.h"

#define REVERSE_SUBSEQUENCE 0
#define SWAP_NODES 1

std::vector<std::pair<int, std::pair<int, int> > > generate_intra_route_inner(const std::vector<int>& cycle) {
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

std::vector<int> local_search_steepest_inner_cycle(std::vector<int> starting_cycle, std::vector<std::vector<int> > &dist_mat) {

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

std::pair<std::vector<int>, std::vector<int>> local_search_steepest_inner(std::vector<int> &cycle_1, std::vector<int> &cycle_2, std::vector<std::vector<int> > &dist_mat) {
    std::vector<int> new_cycle_1 = local_search_steepest_inner_cycle(cycle_1, dist_mat);
    std::vector<int> new_cycle_2 = local_search_steepest_inner_cycle(cycle_2, dist_mat);

    return std::make_pair(new_cycle_1, new_cycle_2);
}

std::vector<int> local_search_greedy_inner_cycle(std::vector<int> starting_cycle, std::vector<std::vector<int> > &dist_mat) {

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

std::pair<std::vector<int>, std::vector<int>> local_search_greedy_inner(std::vector<int> &cycle_1, std::vector<int> &cycle_2, std::vector<std::vector<int> > &dist_mat) {
    std::vector<int> new_cycle_1 = local_search_greedy_inner_cycle(cycle_1, dist_mat);
    std::vector<int> new_cycle_2 = local_search_greedy_inner_cycle(cycle_2, dist_mat);

    return std::make_pair(new_cycle_1, new_cycle_2);
}

std::pair<std::vector<int>, std::vector<int> > swap_nodes_between(std::vector<int> cycle1, std::vector<int> cycle2, int pos_a, int pos_b) {

    std::vector<int> tmp_cycle1 = cycle1;
    std::vector<int> tmp_cycle2 = cycle2;

    int tmp = tmp_cycle1[pos_a];
    tmp_cycle1[pos_a] = tmp_cycle2[pos_b];
    tmp_cycle2[pos_b] = tmp;

    return std::make_pair(tmp_cycle1, tmp_cycle2);
}

std::pair<std::vector<int>, std::vector<int>> local_search_steepest_between(std::vector<int> cycle_1, std::vector<int> cycle_2, std::vector<std::vector<int> > &dist_mat) {

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

std::pair<std::vector<int>, std::vector<int>> local_search_greedy_between(std::vector<int> cycle_1, std::vector<int> cycle_2, std::vector<std::vector<int> > &dist_mat) {

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

std::vector<int> random_search_cycle(std::vector<int> &starting_cycle, std::vector<std::vector<int> > &dist_mat, std::chrono::microseconds time_limit_microseconds) {

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

std::pair<std::vector<int>, std::vector<int>> random_search(std::vector<int> &cycle_1, std::vector<int> &cycle_2, std::vector<std::vector<int> > &dist_mat, int time_limit=100000) {

    std::vector<int> new_cycle_1 = random_search_cycle(cycle_1, dist_mat, std::chrono::microseconds(time_limit / 2));
    std::vector<int> new_cycle_2 = random_search_cycle(cycle_2, dist_mat, std::chrono::microseconds(time_limit / 2));

    return std::make_pair(new_cycle_1, new_cycle_2);
}


int main(const int argc, char** argv) {
    std::string instance; // kroA100.tsp kroB100.tsp
    std::string method; // steepest greedy
    std::string neighbourhood; // inner between
    std::string beginning; // random heuristic
    int run_nr;

    if (argc > 5) {
        instance = argv[1];
        method = argv[2];
        neighbourhood = argv[3];
        beginning = argv[4];
        run_nr = std::stoi(argv[5]);
    } else {
        instance = "kroA100";
        method = "steepest";
        neighbourhood = "inner";
        beginning = "random";
        run_nr = 1;
        std::cerr << "Invalid number of arguments" << std::endl;
    }


    std::map<int, std::pair<int, int> > data = Utils::read_data(instance);
    std::vector<std::vector<int> > dist_mat = Utils::create_distance_matrix(data);

    std::pair<std::vector<int>, std::vector<int>> cycles;
    std::pair<std::vector<int>, std::vector<int>> c;


    if (beginning == "random") {
        cycles = Utils::random_cycles(dist_mat);
    } else if (beginning == "heuristic") {
        cycles = greedy::nearest_neighbour(dist_mat);
    }

    if (method == "steepest" && neighbourhood == "inner") {
        c = local_search_steepest_inner(cycles.first, cycles.second, dist_mat);

    } else if (method == "steepest" && neighbourhood == "between") {
        c = local_search_steepest_between(cycles.first, cycles.second, dist_mat);

    } else if (method == "greedy" && neighbourhood == "inner") {
        c = local_search_greedy_inner(cycles.first, cycles.second, dist_mat);

    } else if (method == "greedy" && neighbourhood == "between") {
        c = local_search_greedy_between(cycles.first, cycles.second, dist_mat);

    } else if (method == "random") {
        c = random_search(cycles.first, cycles.second, dist_mat);
    }

    int length_before = Utils::calculate_cycle_length(dist_mat, cycles.first) + Utils::calculate_cycle_length(dist_mat, cycles.second);
    int length_after = Utils::calculate_cycle_length(dist_mat, c.first) + Utils::calculate_cycle_length(dist_mat, c.second);

    std::string output = "output/" + method + "_" + neighbourhood + "_" + beginning + "_" + instance + ".txt";
    std::ofstream cycle_file(output);

    if (cycle_file.is_open()) {

        cycle_file << run_nr << ": ";
        cycle_file << length_after << " ";

        for (const auto& elem : c.first) {
            cycle_file << elem << " ";
        }

        cycle_file << std::endl;

        cycle_file << run_nr << ": ";
        cycle_file << length_after << " ";

        for (const auto& elem : c.second) {
            cycle_file << elem << " ";
        }

        cycle_file << std::endl;
        cycle_file.close();

        std::cout << "Cycle exported" << std::endl;
    } else {
        std::cerr << "Unable to open for export." << std::endl;
    }


}
