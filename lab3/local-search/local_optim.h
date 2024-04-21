//
// Created by Mariusz on 14.04.2024.
//

#ifndef LOCAL_SEARCH_LOCAL_OPTIM_H
#define LOCAL_SEARCH_LOCAL_OPTIM_H

#define SWAP_EDGE 111
#define SWAP_NODES_BETWEEN_CYCLES 999


#include <utility>

#include "Utils.h"


class local_optim {
    public:

    struct move {
        int type;
        std::vector<int> data;
        int score;
        move(int type, std::vector<int> data, int score) : type(type), data(std::move(data)), score(score) {}

        move() : type(-1), data({}), score(-1) {}

        bool operator==(const move& other) const {
            return type == other.type && data == other.data && score == other.score;
        }
    };

    static std::pair<std::vector<int>, std::vector<int>>
    candidate_moves_algorithm(std::vector<int> cycle_1, std::vector<int> cycle_2, std::vector<std::vector<int> > &dist_mat, int k);

    static std::pair<std::vector<int>, std::vector<int>>
    cache_algorithm(std::vector<int> cycle_1, std::vector<int> cycle_2, std::vector<std::vector<int> > &dist_mat);

    static std::vector<move>
    generate_init_moves(const std::vector<int> &cycle1, const std::vector<int> &cycle2, std::vector<std::vector<int> > &dist_mat);

};


#endif //LOCAL_SEARCH_LOCAL_OPTIM_H
