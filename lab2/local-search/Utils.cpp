//
// Created by Mariusz on 21.03.2024.
//

#include "Utils.h"

std::vector<std::vector<int> > Utils::create_distance_matrix(const std::map<int, std::pair<int, int> > &data) {
    const int dim = (int) data.size();
    std::vector<std::vector<int>> distance_matrix(dim, std::vector<int>(dim, 0));

    for (auto it1 = data.begin(); it1 != data.end(); ++it1) {
        for (auto it2 = it1; it2 != data.end(); ++it2) {

            const int x = it1->second.first - it2->second.first;
            const int y = it1->second.second - it2->second.second;

            const int distance = (int) std::sqrt(x * x + y * y);
            distance_matrix[it1->first - 1][it2->first - 1] = distance;
            distance_matrix[it2->first - 1][it1->first - 1] = distance;
        }
    }

    return distance_matrix;
}

void Utils::print_matrix(const std::vector<std::vector<int> > &matrix) {

    for (const auto &row: matrix) {
        for (const auto &elem: row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

long Utils::calculate_cycle_length(const std::vector<std::vector<int> > &dist_mat, const std::vector<int> &cycle) {
    long length = 0;
    const int dim = (int) cycle.size();

    for (int i = 0; i < dim; i++) {
        length += dist_mat[cycle[i] - 1][cycle[(i + 1) % dim] - 1];
    }
    return length;
}

int Utils::choose_random_node(const int from, const int to) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(from, to - 1);

    return distr(gen);
}

int Utils::choose_new_random_node(const int from, const int to, const int old) {

    int new_random = old;
    while (new_random == old) {
        new_random = Utils::choose_random_node(from, to);
    }

    return new_random;
}

std::pair<int, int> Utils::choose_nearest_neighbour(const std::vector<std::vector<int>>& dist_mat, const int current_node, const std::vector<bool>& visited) {

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

void Utils::incrementVectors(std::vector<int>& cycle1, std::vector<int>& cycle2) {
    std::transform(cycle1.begin(), cycle1.end(), cycle1.begin(), [](int v) { return v + 1; });
    std::transform(cycle2.begin(), cycle2.end(), cycle2.begin(), [](int v) { return v + 1; });
}

std::pair<std::vector<int>, std::vector<int> > Utils::random_cycles(std::vector<std::vector<int> > &dist_mat) {

    const int dim = (int) dist_mat.size();

    std::vector<int> vertices(dim);
    std::iota(vertices.begin(), vertices.end(), 0); // Uzupełnianie wektora wartościami od 0 do dim-1

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vertices.begin(), vertices.end(), g);

    std::vector<int> cycle1(vertices.begin(), vertices.begin() + dim / 2);
    std::vector<int> cycle2(vertices.begin() + dim / 2, vertices.end());

    incrementVectors(cycle1, cycle2);

    return std::make_pair(cycle1, cycle2);
}

std::vector<int> Utils::reverse_cycle(std::vector<int> vec, int start, int end) {

    int size = (int) vec.size();

    start = (start + size) % size;
    end = (end + size) % size;

    if (start > end) {
        std::swap(start, end);
    }

    std::reverse(vec.begin() + start, vec.begin() + end + 1);

    return vec;
}

std::vector<int> Utils::swap_nodes(std::vector<int> init, int pos_a, int pos_b) {
    std::iter_swap(init.begin()+pos_a, init.begin()+pos_b);
    return init;
}

std::map<int, std::pair<int, int> > Utils::read_data(std::string filename) {

    filename = "../" + filename + ".tsp";
    std::ifstream file(filename);
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

    std::cout << "Data read successfully" << std::endl;
    if (data.empty()) {
        std::cerr << "Data is empty" << std::endl;
    }
    return data;
}
