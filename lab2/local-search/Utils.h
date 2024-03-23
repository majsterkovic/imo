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

};



#endif //UTILS_H
