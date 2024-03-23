//
// Created by Mariusz on 21.03.2024.
//

#include "Utils.h"

std::vector<std::vector<int> > Utils::create_distance_matrix(const std::map<int, std::pair<int, int> >& data) {
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

void Utils::print_matrix(const std::vector<std::vector<int> >& matrix) {

    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

long Utils::calculate_cycle_length(const std::vector<std::vector<int> >& dist_mat, const std::vector<int>& cycle) {
    long length = 0;
    const int dim = (int) cycle.size();

    for (int i = 0; i < dim; i++) {
        length += dist_mat[cycle[i]-1][cycle[(i + 1) % dim]-1];
    }
    return length;
}

int Utils::choose_random_node(const int from, const int to) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(from, to - 1);

    return distr(gen);
}