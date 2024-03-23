

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
#define REVERSE_SUBSEQUENCE 0
#define SWAP_NODES 1

std::vector<std::pair<int, std::pair<int, int> > > generate_intra_route(const std::vector<int>& cycle) {
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

std::vector<int> local_search_steepest(std::vector<int> starting_cycle, std::vector<std::vector<int> > &dist_mat) {
    for (int i = 0; i < starting_cycle.size(); ++i) {
        starting_cycle[i]--;
    }
    // Wygeneruj wszystkie możliwe wymiany wierzchołków i wymiany krawędzi
    std::vector<std::pair<int, std::pair<int, int> > > moves = generate_intra_route(starting_cycle);

    std::vector<int> tmp;
    int best_index;
    int best_value;
    int subdistance_before;
    int subdistance_after;
    int delta;
    int cycle_size = starting_cycle.size();
    std::pair<int, std::pair<int, int> > move;
    // Oceń losowy ruch, jeżeli jest lepszy niż stan aktualny, to go zastosuj
    while (!moves.empty()) {
        best_index = INT_MIN;
        best_value = INT_MIN;

        for (int i = 0; i < moves.size(); ++i) { // Wybieranie aktualnie najlepszego ruchu
            move = moves[i];
            if (move.first == 0) { // odwracanie podciągu, czyli zamiana krawędzi
                tmp = Utils::reverse_cycle(starting_cycle, move.second.first, move.second.second);

                subdistance_before = dist_mat[starting_cycle[move.second.first]][starting_cycle[
                        (cycle_size + ((move.second.first - 1) % cycle_size)) % cycle_size]]
                                     + dist_mat[starting_cycle[move.second.second]][starting_cycle[
                        (cycle_size + ((move.second.second + 1) % cycle_size)) % cycle_size]];

                subdistance_after =
                        dist_mat[tmp[move.second.first]][tmp[(cycle_size + ((move.second.first - 1) % cycle_size)) %
                                                             cycle_size]]
                        + dist_mat[tmp[move.second.second]][tmp[(cycle_size + ((move.second.second + 1) % cycle_size)) %
                                                                cycle_size]];

                delta = subdistance_before - subdistance_after;

                if (delta >= best_value) {
                    best_value = delta;
                    best_index = i;
                }
            } else if (move.first == 1) { // zamiana wierzchołków
                tmp = Utils::swap_nodes(starting_cycle, move.second.first, move.second.second);

                subdistance_before = dist_mat[starting_cycle[move.second.first]][starting_cycle[
                        (cycle_size + ((move.second.first - 1) % cycle_size)) % cycle_size]]
                                     + dist_mat[starting_cycle[move.second.first]][starting_cycle[
                        (cycle_size + ((move.second.first + 1) % cycle_size)) % cycle_size]]
                                     + dist_mat[starting_cycle[move.second.second]][starting_cycle[
                        (cycle_size + ((move.second.second - 1) % cycle_size)) % cycle_size]]
                                     + dist_mat[starting_cycle[move.second.second]][starting_cycle[
                        (cycle_size + ((move.second.second + 1) % cycle_size)) % cycle_size]];

                subdistance_after =
                        dist_mat[tmp[move.second.first]][tmp[(cycle_size + ((move.second.first - 1) % cycle_size)) %
                                                             cycle_size]]
                        + dist_mat[tmp[move.second.first]][tmp[(cycle_size + ((move.second.first + 1) % cycle_size)) %
                                                               cycle_size]]
                        + dist_mat[tmp[move.second.second]][tmp[(cycle_size + ((move.second.second - 1) % cycle_size)) %
                                                                cycle_size]]
                        + dist_mat[tmp[move.second.second]][tmp[(cycle_size + ((move.second.second + 1) % cycle_size)) %
                                                                cycle_size]];

                delta = subdistance_before - subdistance_after;

                if (delta >= best_value) {
                    best_value = delta;
                    best_index = i;
                }
            }
        }

        move = moves[best_index];
        moves.erase(moves.begin()+best_index);

        if (move.first == 0) { // odwracanie podciągu, czyli zamiana krawędzi
            tmp = Utils::reverse_cycle(starting_cycle, move.second.first, move.second.second);

            subdistance_before = dist_mat[starting_cycle[move.second.first]][starting_cycle[(cycle_size + ((move.second.first-1) % cycle_size)) % cycle_size]]
                                 + dist_mat[starting_cycle[move.second.second]][starting_cycle[(cycle_size + ((move.second.second+1) % cycle_size)) % cycle_size]];

            subdistance_after = dist_mat[tmp[move.second.first]][tmp[(cycle_size + ((move.second.first-1) % cycle_size)) % cycle_size]]
                                + dist_mat[tmp[move.second.second]][tmp[(cycle_size + ((move.second.second+1) % cycle_size)) % cycle_size]];

            delta = subdistance_before - subdistance_after;

            if (delta > 0) {
                starting_cycle = tmp;
            }
        }
        else if (move.first == 1) { // zamiana wierzchołków
            tmp = Utils::swap_nodes(starting_cycle, move.second.first, move.second.second);

            subdistance_before = dist_mat[starting_cycle[move.second.first]][starting_cycle[(cycle_size + ((move.second.first-1) % cycle_size)) % cycle_size]]
                                 + dist_mat[starting_cycle[move.second.first]][starting_cycle[(cycle_size + ((move.second.first+1) % cycle_size)) % cycle_size]]
                                 + dist_mat[starting_cycle[move.second.second]][starting_cycle[(cycle_size + ((move.second.second-1) % cycle_size)) % cycle_size]]
                                 + dist_mat[starting_cycle[move.second.second]][starting_cycle[(cycle_size + ((move.second.second+1) % cycle_size)) % cycle_size]];

            subdistance_after = dist_mat[tmp[move.second.first]][tmp[(cycle_size + ((move.second.first-1) % cycle_size)) % cycle_size]]
                                + dist_mat[tmp[move.second.first]][tmp[(cycle_size + ((move.second.first+1) % cycle_size)) % cycle_size]]
                                + dist_mat[tmp[move.second.second]][tmp[(cycle_size + ((move.second.second-1) % cycle_size)) % cycle_size]]
                                + dist_mat[tmp[move.second.second]][tmp[(cycle_size + ((move.second.second+1) % cycle_size)) % cycle_size]];

            delta = subdistance_before - subdistance_after;

            if (delta > 0) {
                starting_cycle = tmp;
            }
        }
    }
    for (int i = 0; i < starting_cycle.size(); ++i) {
        starting_cycle[i]++;
    }
    return starting_cycle;
}

std::vector<int> local_search_greedy(std::vector<int> starting_cycle, std::vector<std::vector<int> > &dist_mat) {
    for (int i = 0; i < starting_cycle.size(); ++i) {
        starting_cycle[i]--;
    }
    // Wygeneruj wszystkie możliwe wymiany wierzchołków i wymiany krawędzi
    std::vector<std::pair<int, std::pair<int, int> > > moves = generate_intra_route(starting_cycle);

    std::vector<int> tmp;
    int random;
    int subdistance_before;
    int subdistance_after;
    int delta;
    int cycle_size = starting_cycle.size();
    std::pair<int, std::pair<int, int> > move;
    // Oceń losowy ruch, jeżeli jest lepszy niż stan aktualny, to go zastosuj
    while (!moves.empty()) {
        random = Utils::choose_random_node(0, moves.size());
        move = moves[random];
        moves.erase(moves.begin()+random);

        if (move.first == 0) { // odwracanie podciągu, czyli zamiana krawędzi
            tmp = Utils::reverse_cycle(starting_cycle, move.second.first, move.second.second);

            subdistance_before = dist_mat[starting_cycle[move.second.first]][starting_cycle[(cycle_size + ((move.second.first-1) % cycle_size)) % cycle_size]]
                    + dist_mat[starting_cycle[move.second.second]][starting_cycle[(cycle_size + ((move.second.second+1) % cycle_size)) % cycle_size]];

            subdistance_after = dist_mat[tmp[move.second.first]][tmp[(cycle_size + ((move.second.first-1) % cycle_size)) % cycle_size]]
                    + dist_mat[tmp[move.second.second]][tmp[(cycle_size + ((move.second.second+1) % cycle_size)) % cycle_size]];

            delta = subdistance_before - subdistance_after;

            if (delta > 0) {
                starting_cycle = tmp;
            }
        }
        else if (move.first == 1) { // zamiana wierzchołków
            tmp = Utils::swap_nodes(starting_cycle, move.second.first, move.second.second);

            subdistance_before = dist_mat[starting_cycle[move.second.first]][starting_cycle[(cycle_size + ((move.second.first-1) % cycle_size)) % cycle_size]]
                                 + dist_mat[starting_cycle[move.second.first]][starting_cycle[(cycle_size + ((move.second.first+1) % cycle_size)) % cycle_size]]
                                 + dist_mat[starting_cycle[move.second.second]][starting_cycle[(cycle_size + ((move.second.second-1) % cycle_size)) % cycle_size]]
                                 + dist_mat[starting_cycle[move.second.second]][starting_cycle[(cycle_size + ((move.second.second+1) % cycle_size)) % cycle_size]];

            subdistance_after = dist_mat[tmp[move.second.first]][tmp[(cycle_size + ((move.second.first-1) % cycle_size)) % cycle_size]]
                                + dist_mat[tmp[move.second.first]][tmp[(cycle_size + ((move.second.first+1) % cycle_size)) % cycle_size]]
                                + dist_mat[tmp[move.second.second]][tmp[(cycle_size + ((move.second.second-1) % cycle_size)) % cycle_size]]
                                + dist_mat[tmp[move.second.second]][tmp[(cycle_size + ((move.second.second+1) % cycle_size)) % cycle_size]];

            delta = subdistance_before - subdistance_after;

            if (delta > 0) {
                starting_cycle = tmp;
            }
        }
    }
    for (int i = 0; i < starting_cycle.size(); ++i) {
        starting_cycle[i]++;
    }
    return starting_cycle;
}


int main(const int argc, char** argv) {
    std::string instance;
    std::string method;
    int run_nr;

    if (argc > 2) {
        instance = argv[1];
        method = argv[2];
        run_nr = std::stoi(argv[3]);
        std::cout << "Filename: " << instance << std::endl;
        std::cout << "Method: " << method << std::endl;
    } else {
        instance = "kroA100.tsp";
        method = "steepest";
        run_nr = 1;
//        std::cout << "Invalid number of arguments" << std::endl;
//        return 1;
    }


    std::map<int, std::pair<int, int> > data = Utils::read_data(instance);

    std::vector<std::vector<int> > dist_mat = Utils::create_distance_matrix(data);
    std::pair<std::vector<int>, std::vector<int> > c;

    c = Utils::random_cycles(dist_mat);

    std::cout << "Cycle 1: ";
    for (const auto& elem : c.first) {
        std::cout << elem << " ";
    }

    std::cout << std::endl;
    std::cout << "Cycle 1 length: " << Utils::calculate_cycle_length(dist_mat, c.first) << std::endl;

    std::cout << "Cycle 2: ";
    for (const auto& elem : c.second) {
        std::cout << elem << " ";
    }

    std::cout << std::endl;
    std::cout << "Cycle 2 length: " << Utils::calculate_cycle_length(dist_mat, c.second) << std::endl;

    std::cout << std::endl << "Trwa algorytm lokalnego przeszukiwania..." << std::endl << std::endl;

    c.first = local_search_steepest(c.first, dist_mat);
    c.second = local_search_steepest(c.second, dist_mat);

//    c.first = local_search_greedy(c.first, dist_mat);
//    c.second = local_search_greedy(c.second, dist_mat);

    std::cout << "Cycle 1: ";
    for (const auto& elem : c.first) {
        std::cout << elem << " ";
    }

    std::cout << std::endl;
    std::cout << "Cycle 1 length: " << Utils::calculate_cycle_length(dist_mat, c.first) << std::endl;

    std::cout << "Cycle 2: ";
    for (const auto& elem : c.second) {
        std::cout << elem << " ";
    }

    std::cout << std::endl;
    std::cout << "Cycle 2 length: " << Utils::calculate_cycle_length(dist_mat, c.second) << std::endl;

    return 0;

}
