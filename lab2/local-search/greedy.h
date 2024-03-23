//
// Created by Mariusz on 23.03.2024.
//

#ifndef LOCAL_SEARCH_GREEDY_H
#define LOCAL_SEARCH_GREEDY_H

#include "Utils.h"
#include "greedy.h"

class greedy {
public:
    static std::pair<std::vector<int>, std::vector<int>> nearest_neighbour(std::vector<std::vector<int>> &dist_mat);
    static std::pair<std::vector<int>, std::vector<int>> regret_heuristic(const std::vector<std::vector<int>>& distance_matrix, int k=2);
    static std::pair<std::vector<int>, std::vector<int>> greedy_cycle(std::vector<std::vector<int>> &dist_mat);


};


#endif //LOCAL_SEARCH_GREEDY_H
