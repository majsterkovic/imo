#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <random>

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

void greedy_nearest_neighbour(std::vector<std::vector<int>> &dist_mat) {

    std::vector<int> cycle1;
    std::vector<int> cycle2;

    const int dim = (int) dist_mat.size();
    std::vector<bool> visited(dim, false);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, dim - 1);

    const int random_index = distr(gen);
    const int random_index2 = distr(gen);

    cycle1.push_back(random_index);
    cycle2.push_back(random_index2);
    visited[random_index] = true;
    visited[random_index2] = true;

    // while there are still unvisited nodes

    while(std::any_of(visited.begin(), visited.end(), [](bool v){ return !v; })) {

        int min_distance = INT_MAX;
        int min_index = -1;
        int min_verticle = -1;

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
        cycle1.insert(cycle1.begin() + min_index, min_verticle);


    }


}

void greedy_cycle() {}

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



    return 0;
}
