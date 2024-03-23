

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


int choose_new_random_node(const int from, const int to, const int old) {

    int new_random = old;
    while (new_random == old) {
        new_random = Utils::choose_random_node(from, to);
    }

    return new_random;
}

//std::pair<std::vector<int>, std::vector<int>> nearest_neighbour(std::vector<std::vector<int>> &dist_mat) {
//
//    int min_distance = INT_MAX;
//    int min_index = -1;
//    int min_verticle = -1;
//
//    std::vector<int> cycle1;
//    std::vector<int> cycle2;
//
//    const int dim = (int) dist_mat.size();
//    std::vector visited(dim, false);
//
//    // wybierz (np. losowo) wierzchołek startowy
//    // wybierz najbliższy wierzchołek i stwórz z tych dwóch wierzchołków niepełny cykl
//
//    const int random_index = choose_new_random_node(0, dim, -1);
//    cycle1.push_back(random_index);
//    visited[random_index] = true;
//
//    const int random_index2 = choose_new_random_node(0, dim, random_index);
//    cycle2.push_back(random_index2);
//    visited[random_index2] = true;
//
//    // dopóki nie zostały dodane wszystkie wierzchołki
//
//    while(!std::all_of(visited.begin(), visited.end(), [](bool v){ return v; })) {
//
//        min_distance = INT_MAX;
//        min_index = -1;
//        min_verticle = -1;
//
//        const int c1_size = (int) cycle1.size();
//        for(int j = 0; j < c1_size; j++) {
//
//            std::pair<int, int> result = choose_nearest_neighbour(dist_mat, cycle1[j], visited);
//
//            const int distance = result.second;
//            const int i = result.first;
//
//            if (distance < min_distance) {
//                min_distance = distance;
//                min_index = j;
//                min_verticle = i;
//            }
//        }
//
//        cycle1.insert(cycle1.begin() + min_index + 1, min_verticle);
//        visited[min_verticle] = true;
//
//        min_distance = INT_MAX;
//        min_index = -1;
//        min_verticle = -1;
//
//        const int c2_size = (int) cycle2.size();
//        for(int j = 0; j < c2_size; j++) {
//
//            std::pair<int, int> result = choose_nearest_neighbour(dist_mat, cycle2[j], visited);
//
//            const int distance = result.second;
//            const int i = result.first;
//
//            if (distance < min_distance) {
//                min_distance = distance;
//                min_index = j;
//                min_verticle = i;
//            }
//        }
//
//        cycle2.insert(cycle2.begin() + min_index + 1, min_verticle);
//        visited[min_verticle] = true;
//    }
//
//    // increment the index by 1 to match the node id
//    std::transform(cycle1.begin(), cycle1.end(), cycle1.begin(), [](int v) { return v + 1; });
//    std::transform(cycle2.begin(), cycle2.end(), cycle2.begin(), [](int v) { return v + 1; });
//
//    return std::make_pair(cycle1, cycle2);
//}

std::pair<std::vector<int>, std::vector<int> > random_cycles(std::vector<std::vector<int> > &dist_mat) {
    std::vector<int> cycle1;
    std::vector<int> cycle2;

    const int dim = (int) dist_mat.size();

    for (int i = 0; i < dim/2; ++i) {
        cycle1.push_back(i+1);
    }

    for (int i = (int) dim/2; i < dim; ++i) {
        cycle2.push_back(i+1);
    }

    std::srand(std::time(0));
    std::random_shuffle(cycle1.begin(), cycle1.end());
    std::random_shuffle(cycle2.begin(), cycle2.end());

    return std::make_pair(cycle1, cycle2);
}

std::vector<int> reverse_cycle(std::vector<int> vec, int start, int end) {
    int size = vec.size();

    // Sprawdź poprawność indeksów startu i końca
    start = (start % size + size) % size;
    end = (end % size + size) % size;

    // Oblicz długość podciągu
    int length = (end - start + size) % size + 1;

    // Tworzenie tymczasowego wektora dla odwrócenia podciągu
    std::vector<int> temp(length);
    for (int i = 0; i < length; ++i) {
        temp[i] = vec[(start + i) % size];
    }

    // Odwróć podciąg w temp
    std::reverse(temp.begin(), temp.end());

    // Zapisz odwrócony podciąg z powrotem do wektora
    for (int i = 0; i < length; ++i) {
        vec[(start + i) % size] = temp[i];
    }

    return vec;
}

std::vector<int> swap_nodes(std::vector<int> init, int pos_start, int pos_end) {
    std::iter_swap(init.begin()+pos_start, init.begin()+pos_end);
    return init;
}

std::vector<std::pair<int, std::pair<int, int> > > generate_intra_route(std::vector<int> cycle) {
    std::vector<std::pair<int, std::pair<int, int> > > moves;
    // (0, (2, 5)) -> Oznacza odwróć podciąg zaczynając od 2 i kończąc na 5
    // (1, (2, 5)) -> Oznacza zamień wierzchołki na pozycjach 2 i 5

    for (int i = 0; i < cycle.size()-1; ++i) {
        for (int j = i+1; j < cycle.size(); ++j) {
            moves.push_back(std::make_pair(1, std::make_pair(i, j)));
        }
    }

    for (int i = 2; i <= cycle.size()-2; ++i) { // dla każdego rozmiaru okna
        for (int j = 0; j < cycle.size(); ++j) { // przejedź oknem nad wektorem
            moves.push_back(std::make_pair(0, std::make_pair(j, (j+i)%cycle.size())));
        }
    }

    std::srand(std::time(0));
    std::random_shuffle(moves.begin(), moves.end());

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
                tmp = reverse_cycle(starting_cycle, move.second.first, move.second.second);

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
                tmp = swap_nodes(starting_cycle, move.second.first, move.second.second);

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
            tmp = reverse_cycle(starting_cycle, move.second.first, move.second.second);

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
            tmp = swap_nodes(starting_cycle, move.second.first, move.second.second);

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
        random = choose_random_node(0, moves.size());
        move = moves[random];
        moves.erase(moves.begin()+random);

        if (move.first == 0) { // odwracanie podciągu, czyli zamiana krawędzi
            tmp = reverse_cycle(starting_cycle, move.second.first, move.second.second);

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
            tmp = swap_nodes(starting_cycle, move.second.first, move.second.second);

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
        std::cout << "Invalid number of arguments" << std::endl;
    }

    std::ifstream file(instance);
    std::string line;

    std::map<int, std::pair<int, int> > data;

    while (std::getline(file, line)) {
        if (line.find("NODE_COORD_SECTION") != std::string::npos) {
            while (std::getline(file, line)) {

                if (line.find("EOF") != std::string::npos) {
                    break;
                }

                int id, x, y;
                if (std::istringstream iss(line); !(iss >> id >> x >> y)) {
                    std::cout << "Error reading file" << std::endl;
                    break;
                }

                data[id] = std::make_pair(x, y);
            }
        }
    }

    std::vector<std::vector<int> > dist_mat = create_distance_matrix(data);
    std::pair<std::vector<int>, std::vector<int> > c;

    c = random_cycles(dist_mat);

    std::cout << "Cycle 1: ";
    for (const auto& elem : c.first) {
        std::cout << elem << " ";
    }

    std::cout << std::endl;
    std::cout << "Cycle 1 length: " << calculate_cycle_length(dist_mat, c.first) << std::endl;

    std::cout << "Cycle 2: ";
    for (const auto& elem : c.second) {
        std::cout << elem << " ";
    }

    std::cout << std::endl;
    std::cout << "Cycle 2 length: " << calculate_cycle_length(dist_mat, c.second) << std::endl;

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
    std::cout << "Cycle 1 length: " << calculate_cycle_length(dist_mat, c.first) << std::endl;

    std::cout << "Cycle 2: ";
    for (const auto& elem : c.second) {
        std::cout << elem << " ";
    }

    std::cout << std::endl;
    std::cout << "Cycle 2 length: " << calculate_cycle_length(dist_mat, c.second) << std::endl;

    return 0;

}
