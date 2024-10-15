
#include "Utils.h"
#include "greedy.h"
#include "local.h"
#include <filesystem>
namespace fs = std::filesystem;




int main(const int argc, char** argv) {
    std::string instance; // kroA100.tsp kroB100.tsp
    std::string method; // steepest greedy
    std::string neighbourhood; // inner between
    std::string beginning; // random heuristic
    int run_nr;

    if (argc > 5) {
        instance = argv[1];
        method = argv[2];
        neighbourhood = argv[3];
        beginning = argv[4];
        run_nr = std::stoi(argv[5]);
    } else {
        instance = "kroB200";
        method = "steepest";
        neighbourhood = "inner";
        beginning = "random";
        run_nr = 1;
        std::cerr << "Invalid number of arguments" << std::endl;
    }


    std::map<int, std::pair<int, int> > data = Utils::read_data(instance);
    std::vector<std::vector<int> > dist_mat = Utils::create_distance_matrix(data);

    std::pair<std::vector<int>, std::vector<int>> cycles;
    std::pair<std::vector<int>, std::vector<int>> c;

 // measure time of 1000 local_search_greedy

    int start_time = 0;
    int end_time = 0;

    for (int i = 0; i < 1000; i++) {
        start_time = clock();
        r = Utils::random_cycles(dist_mat);

        cycles = greedy::greedy_cycle(dist_mat);
        end_time = clock();

}
