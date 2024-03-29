//
// Created by Mariusz on 23.03.2024.
//

#include "greedy.h"


std::pair<std::vector<int>, std::vector<int>> greedy::nearest_neighbour(std::vector<std::vector<int>> &dist_mat) {

    int min_distance = INT_MAX;
    int min_index = -1;
    int min_verticle = -1;

    std::vector<int> cycle1;
    std::vector<int> cycle2;

    const int dim = (int) dist_mat.size();
    std::vector visited(dim, false);

    const int random_index = Utils::choose_new_random_node(0, dim, -1);
    cycle1.push_back(random_index);
    visited[random_index] = true;

    const int random_index2 = Utils::choose_new_random_node(0, dim, random_index);
    cycle2.push_back(random_index2);
    visited[random_index2] = true;


    while(!std::all_of(visited.begin(), visited.end(), [](bool v){ return v; })) {

        min_distance = INT_MAX;
        min_index = -1;
        min_verticle = -1;

        const int c1_size = (int) cycle1.size();
        for(int j = 0; j < c1_size; j++) {

            std::pair<int, int> result = Utils::choose_nearest_neighbour(dist_mat, cycle1[j], visited);

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

            std::pair<int, int> result = Utils::choose_nearest_neighbour(dist_mat, cycle2[j], visited);

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

    Utils::incrementVectors(cycle1, cycle2);

    return std::make_pair(cycle1, cycle2);
}


std::pair<std::vector<int>, std::vector<int>> greedy::greedy_cycle(std::vector<std::vector<int>> &dist_mat) {

    int min_distance = INT_MAX;
    int min_index = -1;
    int min_verticle = -1;

    std::vector<int> cycle1;
    std::vector<int> cycle2;

    const int dim = (int) dist_mat.size();
    std::vector visited(dim, false);


    const int random_index = Utils::choose_new_random_node(0, dim, -1);
    const int nearest_neighbour = Utils::choose_nearest_neighbour(dist_mat, random_index, visited).first;
    cycle1.push_back(random_index);
    cycle1.push_back(nearest_neighbour);

    visited[random_index] = true;
    visited[nearest_neighbour] = true;

    const int random_index2 = Utils::choose_new_random_node(0, dim, random_index);
    const int nearest_neighbour2 = Utils::choose_nearest_neighbour(dist_mat, random_index2, visited).first;
    cycle2.push_back(random_index2);
    cycle2.push_back(nearest_neighbour2);

    visited[random_index2] = true;
    visited[nearest_neighbour2] = true;


    while(!std::all_of(visited.begin(), visited.end(), [](bool v){ return v; })) {

        min_distance = INT_MAX;
        min_index = -1;
        min_verticle = -1;


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

    Utils::incrementVectors(cycle1, cycle2);

    return std::make_pair(cycle1, cycle2);
}


std::pair<std::vector<int>, std::vector<int>> greedy::regret_heuristic(const std::vector<std::vector<int>>& distance_matrix, int k) {

    const int dim = (int) distance_matrix.size();
    int start1 = Utils::choose_new_random_node(0, dim, -1);
    int start2 = Utils::choose_new_random_node(0, dim, start1);

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

    Utils::incrementVectors(cycle1, cycle2);

    cycle1.pop_back();
    cycle2.pop_back();

    return std::make_pair(cycle1, cycle2);
}