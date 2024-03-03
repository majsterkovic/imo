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
#include <thread>


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

int choose_nearest_neighbour(const std::vector<std::vector<int>>& dist_mat, const int current_node, const std::vector<bool>& visited) {

    int distance = INT_MAX;
    int index = -1;

    for(int i = 0; i < dist_mat.size(); i++) {
        if (i != current_node && visited[i] == false && dist_mat[current_node][i] < distance) {
            distance = dist_mat[current_node][i];
            index = i;
        }
    }

    return index;
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
    const int nearest_neighbour = choose_nearest_neighbour(dist_mat, random_index, visited);
    cycle1.push_back(random_index);
    cycle1.push_back(nearest_neighbour);

    print_cycle(cycle1);

    visited[random_index] = true;
    visited[nearest_neighbour] = true;

    const int random_index2 = choose_new_random_node(0, dim, random_index);
    const int nearest_neighbour2 = choose_nearest_neighbour(dist_mat, random_index2, visited);
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
                //for(int j = 0; j < 1; j++) {
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
                //for(int j = 0; j < 1; j++) {
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

void nearest_neighbour() {}

void regret_heuristic() {}

int main(const int argc, char** argv) {

    std::string filename;

    if (argc > 1) {
        filename = argv[1];
        std::cout << "Filename: " << filename << std::endl;
    } else {
        std::cout << "Invalid number of arguments" << std::endl;
    }

    std::ifstream file(filename);
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


    std::cout << "Data size: " << data.size() << std::endl;

    std::vector dist_mat = create_distance_matrix(data);

    std::cout << "Distance matrix [4,5] [5,4] " << dist_mat[3][4] << " " << dist_mat[4][3] << std::endl;

    std::pair<std::vector<int>, std::vector<int>> c = greedy_cycle(dist_mat);

    std::cout << "Cycle 1: ";
    for (const auto& elem : c.first) {
        std::cout << elem << " ";
    }

    std::cout << std::endl;

    std::cout << "Cycle 2: ";
    for (const auto& elem : c.second) {
        std::cout << elem << " ";
    }

    std::ofstream cycle1_file("cycle1.txt");
    if (cycle1_file.is_open()) {
        for (const auto& elem : c.first) {
            cycle1_file << elem << " ";
        }
        cycle1_file.close();
        std::cout << "Cycle 1 exported to cycle1.txt" << std::endl;
    } else {
        std::cerr << "Unable to open cycle1.txt for export." << std::endl;
    }

    std::ofstream cycle2_file("cycle2.txt");
    if (cycle2_file.is_open()) {
        for (const auto& elem : c.second) {
            cycle2_file << elem << " ";
        }
        cycle2_file.close();
        std::cout << "Cycle 2 exported to cycle2.txt" << std::endl;
    } else {
        std::cerr << "Unable to open cycle2.txt for export." << std::endl;
    }


    return 0;
}
