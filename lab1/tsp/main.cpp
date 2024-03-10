#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <random>
#include <climits>
#include <unordered_set>
#include <limits>
#include <set>

// This is a modification of the TSP problem
// The problem is to find two cycles that cover all the nodes

std::vector<std::vector<int>> create_distance_matrix(const std::map<int, std::pair<int, int>>& data) {

    const int dim = (int) data.size();
    std::vector distance_matrix(dim, std::vector(dim, 0));

    for (auto it1 = data.begin(); it1 != data.end(); ++it1) {
        for (auto it2 = it1; it2 != data.end(); ++it2) {

            int x = it1->second.first - it2->second.first;
            int y = it1->second.second - it2->second.second;

            int distance = (int) std::sqrt(x * x + y * y);
            distance_matrix[it1->first - 1][it2->first - 1] = distance;
            distance_matrix[it2->first - 1][it1->first - 1] = distance;
        }
    }

    return distance_matrix;
}

void print_matrix(const std::vector<std::vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

void print_cycle(const std::vector<int>& cycle) {
    for (const auto& elem : cycle) {
        std::cout << elem << " : ";
    }
    std::cout << std::endl;
}

long calculate_cycle_length(const std::vector<std::vector<int>>& dist_mat, const std::vector<int>& cycle) {

    long length = 0;
    const int dim = (int) cycle.size();

    for (int i = 0; i < dim; i++) {
        length += dist_mat[cycle[i]-1][cycle[(i + 1) % dim]-1];
    }

    return length;
}

int choose_random_node(const int from, const int to) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(from, to - 1);

    return distr(gen);
}

int choose_new_random_node(const int from, const int to, const int old) {

    int new_random = old;
    while (new_random == old) {
        new_random = choose_random_node(from, to);
    }

    return new_random;
}

std::pair<int, int> choose_nearest_neighbour(const std::vector<std::vector<int>>& dist_mat, const int current_node, const std::vector<bool>& visited) {

    int distance = INT_MAX;
    int index = -1;

    for(int i = 0; i < dist_mat.size(); i++) {
        if (!visited[i]) {
            if (i != current_node && dist_mat[current_node][i] < distance) {
                distance = dist_mat[current_node][i];
                index = i;
            }
        }
    }

    return std::make_pair(index, distance);
}

std::pair<std::vector<int>, std::vector<int>> greedy_cycle(std::vector<std::vector<int>> &dist_mat) {

    int min_distance = INT_MAX;
    int min_index = -1;
    int min_verticle = -1;

    std::vector<int> cycle1;
    std::vector<int> cycle2;

    const int dim = (int) dist_mat.size();
    std::vector visited(dim, false);

    // wybierz (np. losowo) wierzchołek startowy
    // wybierz najbliższy wierzchołek i stwórz z tych dwóch wierzchołków niepełny cykl

    const int random_index = choose_new_random_node(0, dim, -1);
    const int nearest_neighbour = choose_nearest_neighbour(dist_mat, random_index, visited).first;
    cycle1.push_back(random_index);
    cycle1.push_back(nearest_neighbour);

    visited[random_index] = true;
    visited[nearest_neighbour] = true;

    const int random_index2 = choose_new_random_node(0, dim, random_index);
    const int nearest_neighbour2 = choose_nearest_neighbour(dist_mat, random_index2, visited).first;
    cycle2.push_back(random_index2);
    cycle2.push_back(nearest_neighbour2);

    visited[random_index2] = true;
    visited[nearest_neighbour2] = true;

    // dopóki nie zostały dodane wszystkie wierzchołki

    while(!std::all_of(visited.begin(), visited.end(), [](bool v){ return v; })) {

        min_distance = INT_MAX;
        min_index = -1;
        min_verticle = -1;

        ///wstaw do cyklu wierzchołek, który minimalizuje długość cyklu

        for (int i = 0; i < dim; i++) {
            if (!visited[i]) {

                const int c1_size = (int) cycle1.size();
                for(int j = 0; j < c1_size; j++) {
                    const int next_j = (j + 1) % c1_size;
                    int distance_update = dist_mat[cycle1[j]][i] + dist_mat[i][cycle1[next_j]] - dist_mat[cycle1[j]][cycle1[next_j]];

                    if (distance_update < min_distance) {
                        min_distance = distance_update;
                        min_index = j;
                        min_verticle = i;
                    }
                }
            }
        }

        cycle1.insert(cycle1.begin() + min_index + 1, min_verticle);
        visited[min_verticle] = true;

        min_distance = INT_MAX;
        min_index = -1;
        min_verticle = -1;

        for (int i = 0; i < dim; i++) {
            if (!visited[i]) {

                const int c2_size = (int) cycle2.size();
                for(int j = 0; j < c2_size; j++) {
                    const int next_j = (j + 1) % c2_size;
                    int distance_update = dist_mat[cycle2[j]][i] + dist_mat[i][cycle2[next_j]] - dist_mat[cycle2[j]][cycle2[next_j]];

                    if (distance_update < min_distance) {
                        min_distance = distance_update;
                        min_index = j;
                        min_verticle = i;
                    }
                }
            }
        }

        cycle2.insert(cycle2.begin() + min_index + 1, min_verticle);
        visited[min_verticle] = true;

    }

    // increment the index by 1 to match the node id
    std::transform(cycle1.begin(), cycle1.end(), cycle1.begin(), [](int v) { return v + 1; });
    std::transform(cycle2.begin(), cycle2.end(), cycle2.begin(), [](int v) { return v + 1; });

    return std::make_pair(cycle1, cycle2);
}

std::pair<std::vector<int>, std::vector<int>> nearest_neighbour(std::vector<std::vector<int>> &dist_mat) {

    int min_distance = INT_MAX;
    int min_index = -1;
    int min_verticle = -1;

    std::vector<int> cycle1;
    std::vector<int> cycle2;

    const int dim = (int) dist_mat.size();
    std::vector visited(dim, false);

    // wybierz (np. losowo) wierzchołek startowy
    // wybierz najbliższy wierzchołek i stwórz z tych dwóch wierzchołków niepełny cykl

    const int random_index = choose_new_random_node(0, dim, -1);
    cycle1.push_back(random_index);
    visited[random_index] = true;

    const int random_index2 = choose_new_random_node(0, dim, random_index);
    cycle2.push_back(random_index2);
    visited[random_index2] = true;

    // dopóki nie zostały dodane wszystkie wierzchołki

    while(!std::all_of(visited.begin(), visited.end(), [](bool v){ return v; })) {

        min_distance = INT_MAX;
        min_index = -1;
        min_verticle = -1;

        const int c1_size = (int) cycle1.size();
        for(int j = 0; j < c1_size; j++) {

            std::pair<int, int> result = choose_nearest_neighbour(dist_mat, cycle1[j], visited);

            const int distance = result.second;
            const int i = result.first;

            if (distance < min_distance) {
                min_distance = distance;
                min_index = j;
                min_verticle = i;
            }
        }

        cycle1.insert(cycle1.begin() + min_index + 1, min_verticle);
        visited[min_verticle] = true;

        min_distance = INT_MAX;
        min_index = -1;
        min_verticle = -1;

        const int c2_size = (int) cycle2.size();
        for(int j = 0; j < c2_size; j++) {

            std::pair<int, int> result = choose_nearest_neighbour(dist_mat, cycle2[j], visited);

            const int distance = result.second;
            const int i = result.first;

            if (distance < min_distance) {
                min_distance = distance;
                min_index = j;
                min_verticle = i;
            }
        }

        cycle2.insert(cycle2.begin() + min_index + 1, min_verticle);
        visited[min_verticle] = true;
    }

    // increment the index by 1 to match the node id
    std::transform(cycle1.begin(), cycle1.end(), cycle1.begin(), [](int v) { return v + 1; });
    std::transform(cycle2.begin(), cycle2.end(), cycle2.begin(), [](int v) { return v + 1; });

    return std::make_pair(cycle1, cycle2);
}


std::pair<std::vector<int>, std::vector<int>> regret_heuristic(const std::vector<std::vector<int>>& distance_matrix, int k=2) {

    const int dim = (int) distance_matrix.size();
    int start1 = choose_new_random_node(0, dim, -1);
    int start2 = choose_new_random_node(0, dim, start1);

    std::set<int> unvisited_cities;
    for (int i = 0; i < distance_matrix.size(); ++i) {
        if (i != start1 && i != start2)
            unvisited_cities.insert(i);
    }

    std::vector<int> cycle1 = {start1, start1};
    double total_length1 = 0;

    std::vector<int> cycle2 = {start2, start2};
    double total_length2 = 0;

    while (cycle1.size() < dim /2) {
        std::vector<std::tuple<double, int, int, double>> regrets;
        for (int city : unvisited_cities) {
            std::vector<std::tuple<double, int>> insertion_data;
            for (int i = 1; i < cycle1.size(); ++i) {
                double length = total_length1 - distance_matrix[cycle1[i - 1]][cycle1[i]] +
                                distance_matrix[cycle1[i - 1]][city] + distance_matrix[city][cycle1[i]];
                insertion_data.emplace_back(length, i);
            }
            std::sort(insertion_data.begin(), insertion_data.end(), [](const auto& a, const auto& b) {
                return std::get<0>(a) < std::get<0>(b);
            });
            double regret;
            double best_length;
            int best_i;
            if (insertion_data.size() >= k) {
                regret = std::get<0>(insertion_data[k - 1]) - std::get<0>(insertion_data[0]);
                best_length = std::get<0>(insertion_data[0]);
                best_i = std::get<1>(insertion_data[0]);
            } else {
                regret = -std::get<0>(insertion_data[0]);
                best_length = std::get<0>(insertion_data[0]);
                best_i = std::get<1>(insertion_data[0]);
            }
            regrets.emplace_back(regret, city, best_i, best_length);
        }

        auto max_regret = std::max_element(regrets.begin(), regrets.end());
        int best_city = std::get<1>(*max_regret);
        int best_i = std::get<2>(*max_regret);
        total_length1 = std::get<3>(*max_regret);
        cycle1.insert(cycle1.begin() + best_i, best_city);
        unvisited_cities.erase(best_city);
    }

    while (!unvisited_cities.empty()) {
        std::vector<std::tuple<double, int, int, double>> regrets;
        for (int city : unvisited_cities) {
            std::vector<std::tuple<double, int>> insertion_data;
            for (int i = 1; i < cycle2.size(); ++i) {
                double length = total_length2 - distance_matrix[cycle2[i - 1]][cycle2[i]] +
                                distance_matrix[cycle2[i - 1]][city] + distance_matrix[city][cycle2[i]];
                insertion_data.emplace_back(length, i);
            }
            std::sort(insertion_data.begin(), insertion_data.end(), [](const auto& a, const auto& b) {
                return std::get<0>(a) < std::get<0>(b);
            });
            double regret;
            double best_length;
            int best_i;
            if (insertion_data.size() >= k) {
                regret = std::get<0>(insertion_data[k - 1]) - std::get<0>(insertion_data[0]);
                best_length = std::get<0>(insertion_data[0]);
                best_i = std::get<1>(insertion_data[0]);
            } else {
                regret = -std::get<0>(insertion_data[0]);
                best_length = std::get<0>(insertion_data[0]);
                best_i = std::get<1>(insertion_data[0]);
            }
            regrets.emplace_back(regret, city, best_i, best_length);
        }

        auto max_regret = std::max_element(regrets.begin(), regrets.end());
        int best_city = std::get<1>(*max_regret);
        int best_i = std::get<2>(*max_regret);
        total_length2 = std::get<3>(*max_regret);
        cycle2.insert(cycle2.begin() + best_i, best_city);
        unvisited_cities.erase(best_city);
    }

    std::transform(cycle1.begin(), cycle1.end(), cycle1.begin(), [](int v) { return v + 1; });
    std::transform(cycle2.begin(), cycle2.end(), cycle2.begin(), [](int v) { return v + 1; });

    cycle1.pop_back();
    cycle2.pop_back();

    return std::make_pair(cycle1, cycle2);
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

    std::map<int, std::pair<int, int>> data;


    // Read the file to data vector
    //     NAME: kroB100
    // TYPE: TSP
    // COMMENT: 100-city problem B (Krolak/Felts/Nelson)
    // DIMENSION: 100
    // EDGE_WEIGHT_TYPE : EUC_2D
    // NODE_COORD_SECTION
    // 1 3140 1401
    // 2 556 1056
    // 3 3675 1522

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

    std::vector dist_mat = create_distance_matrix(data);
    std::pair<std::vector<int>, std::vector<int>> c;

    auto start = std::chrono::high_resolution_clock::now();

    if (method == "nearest_neighbour") {
        c = nearest_neighbour(dist_mat);
    } else if (method == "greedy_cycle") {
        c = greedy_cycle(dist_mat);
    } else if (method == "regret_heuristic") {
        c = regret_heuristic(dist_mat, 2);
    } else {
        std::cerr << "Invalid method" << std::endl;
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::ofstream time_file("time_" + method + "_" + instance + ".txt", std::ios_base::app);
    if (time_file.is_open()) {
        time_file << run_nr << ":" << duration.count() << std::endl;
        time_file.close();
    }

    std::cout << "Cycle 1: ";
    for (const auto& elem : c.first) {
        std::cout << elem << " ";
    }

    std::cout << std::endl;

    std::cout << "Cycle 2: ";
    for (const auto& elem : c.second) {
        std::cout << elem << " ";
    }

    std::cout << std::endl;

    std::cout << "Cycle 1 length: " << calculate_cycle_length(dist_mat, c.first) << std::endl;
    std::cout << "Cycle 2 length: " << calculate_cycle_length(dist_mat, c.second) << std::endl;


    std::ofstream cycle_file("cycle_" + method + "_" + instance + ".txt", std::ios_base::app);
    if (cycle_file.is_open()) {

        cycle_file << run_nr << ": ";
        cycle_file << calculate_cycle_length(dist_mat, c.first) << " ";

        for (const auto& elem : c.first) {
            cycle_file << elem << " ";
        }

        cycle_file << std::endl;

        cycle_file << run_nr << ": ";
        cycle_file << calculate_cycle_length(dist_mat, c.first) << " ";

        for (const auto& elem : c.second) {
            cycle_file << elem << " ";
        }

        cycle_file << std::endl;
        cycle_file.close();

        std::cout << "Cycle exported" << std::endl;
    } else {
        std::cerr << "Unable to open for export." << std::endl;
    }

    return 0;
}
