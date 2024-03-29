//
// Created by Mariusz on 21.03.2024.
//

#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cmath>
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


class Utils {
public:
    static std::vector<std::vector<int>> create_distance_matrix(const std::map<int, std::pair<int, int> > &data);
    static void print_matrix(const std::vector<std::vector<int> > &matrix);
    static long calculate_cycle_length(const std::vector<std::vector<int> > &dist_mat, const std::vector<int> &cycle);
    static int choose_random_node(const int from, const int to);
    static int choose_new_random_node(const int from, const int to, const int old);
    static std::pair<int, int> choose_nearest_neighbour(const std::vector<std::vector<int>>& dist_mat, const int current_node, const std::vector<bool>& visited);
    static void incrementVectors(std::vector<int>& cycle1, std::vector<int>& cycle2);

    static std::pair<std::vector<int>, std::vector<int> > random_cycles(std::vector<std::vector<int> > &dist_mat);

    static std::vector<int> reverse_cycle(std::vector<int> vec, int start, int end);
    static std::vector<int> swap_nodes(std::vector<int> init, int pos_start, int pos_end);

    static std::map<int, std::pair<int, int> > read_data(std::string filename);
};





#endif //UTILS_H
