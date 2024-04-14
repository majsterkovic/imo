//
// Created by Mariusz on 23.03.2024.
//

#ifndef LOCAL_SEARCH_LOCAL_H
#define LOCAL_SEARCH_LOCAL_H

#define REVERSE_SUBSEQUENCE 0
#define SWAP_NODES 1

#include "Utils.h"


class local {
public:
    static std::vector<std::pair<int, std::pair<int, int>>>
    generate_intra_route_inner(const std::vector<int>& cycle);

    static std::vector<int>
    local_search_steepest_inner_cycle(std::vector<int> starting_cycle, std::vector<std::vector<int> > &dist_mat);

    static std::pair<std::vector<int>, std::vector<int>>
    local_search_steepest_inner(std::vector<int> &cycle_1, std::vector<int> &cycle_2, std::vector<std::vector<int> > &dist_mat);


    static std::vector<int>
    local_search_greedy_inner_cycle(std::vector<int> starting_cycle, std::vector<std::vector<int> > &dist_mat);

    static std::pair<std::vector<int>, std::vector<int>>
    local_search_greedy_inner(std::vector<int> &cycle_1, std::vector<int> &cycle_2, std::vector<std::vector<int> > &dist_mat);

    static std::pair<std::vector<int>, std::vector<int>>
    swap_nodes_between(std::vector<int> cycle1, std::vector<int> cycle2, int pos_a, int pos_b);

    static std::pair<std::vector<int>, std::vector<int>>
    local_search_steepest_between(std::vector<int> cycle_1, std::vector<int> cycle_2, std::vector<std::vector<int> > &dist_mat);

    static std::pair<std::vector<int>, std::vector<int>>
    local_search_greedy_between(std::vector<int> cycle_1, std::vector<int> cycle_2, std::vector<std::vector<int> > &dist_mat);

    static std::vector<int>
    random_search_cycle(std::vector<int> &starting_cycle, std::vector<std::vector<int> > &dist_mat, std::chrono::microseconds time_limit_microseconds);

    static std::pair<std::vector<int>, std::vector<int>>
    random_search(std::vector<int> &cycle_1, std::vector<int> &cycle_2, std::vector<std::vector<int> > &dist_mat, int time_limit=100000);


};


#endif //LOCAL_SEARCH_LOCAL_H
