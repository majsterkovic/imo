

/*
 Zadanie polega na implementacji lokalnego przeszukiwania w wersjach stromej (steepest) i
zachłannej (greedy), z dwoma różnym rodzajami sąsiedztwa, starując albo z rozwiązań losowych, albo
z rozwiązań uzyskanych za pomocą jednej z heurystyk opracowanych w ramach poprzedniego
zadania. W sumie 8 kombinacji - wersji lokalnego przeszukiwania.
Jako punkt odniesienia należy zaimplementować algorytm losowego błądzenia, który w każdej iteracji
wykonuje losowo wybrany ruch (niezależnie od jego oceny) i zwraca najlepsze znalezione w ten
sposób rozwiązanie. Algorytm ten powinien działać w takim samym czasie jak średnio najwolniejsza z
wersji lokalnego przeszukiwania.
Sąsiedztwa
W przypadku rozważanego problemu potrzebne będą dwa typy ruchów: ruchy zmieniające zbiory
wierzchołków tworzące dwa cykle i ruchy wewnątrztrasowe, które jedynie zmieniają kolejność
wierzchołków na trasie.
2 - różne starty
2 - wersje algorytmu
2 - ruchy wewnątrz trasy / zmieniające zbiory wierzchołków

Rozaje ruchów:
wymiana dwóch wierzchołków (zamiana dwóch cyfr miejscami)
wymiana dwóch krawędzi (odwrócenie podciągu o długości co najmniej 2 ale co najmniej 2 elementy powinny zostać na swoich miejscach)

1 2 3 4 5 6 7 8 9
1 2 6 5 4 3 7 8 9
 */

//#include <pstl/parallel_backend_utils.h>

#include "Utils.h"
#include "greedy.h"
#include "local.h"
#include "local_optim.h"
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
        instance = "kroA200";
        method = "steepest";
        neighbourhood = "inner";
        beginning = "random";
        run_nr = 1;
        std::cerr << "Invalid number of arguments" << std::endl;
    }


    std::map<int, std::pair<int, int> > data = Utils::read_data(instance);
    std::vector<std::vector<int> > dist_mat = Utils::create_distance_matrix(data);

    std::pair<std::vector<int>, std::vector<int>> cycles_random;
    std::pair<std::vector<int>, std::vector<int>> cycles_heuristic;
    std::pair<std::vector<int>, std::vector<int>> cycles_localsearch;
    std::pair<std::vector<int>, std::vector<int>> cycles_cand;
    std::pair<std::vector<int>, std::vector<int>> cycles_cache;


    cycles_random = Utils::random_cycles(dist_mat);
    long total_length = Utils::calculate_cycle_length(dist_mat, cycles_random.first) + Utils::calculate_cycle_length(dist_mat, cycles_random.second);

    std::string directoryPath = "output/random";
    std::string fileName = instance + ".txt";

    fs::path filePath = fs::path(directoryPath) / fileName;

    if (!fs::exists(directoryPath)) {
        fs::create_directories(filePath.parent_path());
    }

    std::ofstream cycle_file(filePath, std::ios::app);

    if (cycle_file.is_open()) {

        cycle_file << run_nr << ": ";
        cycle_file << total_length << " ";

        for (const auto& elem : cycles_random.first) {
            cycle_file << elem << " ";
        }

        cycle_file << std::endl;

        cycle_file << run_nr << ": ";
        cycle_file << total_length << " ";

        for (const auto& elem : cycles_random.second) {
            cycle_file << elem << " ";
        }

        cycle_file << std::endl;
        cycle_file.close();

        std::cout << "Cycle exported" << std::endl;
    } else {
        std::cerr << "Unable to open for export." << std::endl;
    }

    auto start = std::chrono::high_resolution_clock::now();
    cycles_heuristic = greedy::regret_heuristic(dist_mat, 2);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    total_length = Utils::calculate_cycle_length(dist_mat, cycles_heuristic.first) + Utils::calculate_cycle_length(dist_mat, cycles_heuristic.second);

    directoryPath = "output/heuristic";
    fileName = instance + ".txt";

    filePath = fs::path(directoryPath) / fileName;

    if (!fs::exists(directoryPath)) {
        fs::create_directories(filePath.parent_path());
    }

    std::ofstream cycle_file2(filePath, std::ios::app);

    if (cycle_file2.is_open()) {

        cycle_file2 << run_nr << ": ";
        cycle_file2 << total_length << " ";
        cycle_file2 << duration.count() << " ";

        for (const auto& elem : cycles_heuristic.first) {
            cycle_file2 << elem << " ";
        }

        cycle_file2 << std::endl;

        cycle_file2 << run_nr << ": ";
        cycle_file2 << total_length << " ";
        cycle_file2 << duration.count() << " ";

        for (const auto& elem : cycles_heuristic.second) {
            cycle_file2 << elem << " ";
        }

        cycle_file2 << std::endl;
        cycle_file2.close();

        std::cout << "Cycle exported" << std::endl;
    } else {
        std::cerr << "Unable to open for export." << std::endl;
    }


    start = std::chrono::high_resolution_clock::now();
    cycles_localsearch = local::local_search_steepest_between(cycles_random.first, cycles_random.second, dist_mat);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    total_length = Utils::calculate_cycle_length(dist_mat, cycles_localsearch.first) + Utils::calculate_cycle_length(dist_mat, cycles_localsearch.second);

    directoryPath = "output/local_search_steepest";
    fileName = instance + ".txt";

    filePath = fs::path(directoryPath) / fileName;

    if (!fs::exists(directoryPath)) {
        fs::create_directories(filePath.parent_path());
    }

    std::ofstream cycle_file3(filePath, std::ios::app);

    if (cycle_file3.is_open()) {

        cycle_file3 << run_nr << ": ";
        cycle_file3 << total_length << " ";
        cycle_file3 << duration.count() << " ";

        for (const auto& elem : cycles_localsearch.first) {
            cycle_file3 << elem << " ";
        }

        cycle_file3 << std::endl;

        cycle_file3 << run_nr << ": ";
        cycle_file3 << total_length << " ";
        cycle_file3 << duration.count() << " ";

        for (const auto& elem : cycles_localsearch.second) {
            cycle_file3 << elem << " ";
        }

        cycle_file3 << std::endl;
        cycle_file3.close();

        std::cout << "Cycle exported" << std::endl;
    } else {
        std::cerr << "Unable to open for export." << std::endl;
    }


    start = std::chrono::high_resolution_clock::now();
    cycles_cand = local_optim::candidate_moves_algorithm(cycles_random.first, cycles_random.second, dist_mat, 10);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    total_length = Utils::calculate_cycle_length(dist_mat, cycles_cand.first) + Utils::calculate_cycle_length(dist_mat, cycles_cand.second);

    directoryPath = "output/candidate_moves";
    fileName = instance + ".txt";

    filePath = fs::path(directoryPath) / fileName;

    if (!fs::exists(directoryPath)) {
        fs::create_directories(filePath.parent_path());
    }

    std::ofstream cycle_file4(filePath, std::ios::app);

    if (cycle_file4.is_open()) {

        cycle_file4 << run_nr << ": ";
        cycle_file4 << total_length << " ";
        cycle_file4 << duration.count() << " ";

        for (const auto& elem : cycles_cand.first) {
            cycle_file4 << elem << " ";
        }

        cycle_file4 << std::endl;

        cycle_file4 << run_nr << ": ";
        cycle_file4 << total_length << " ";
        cycle_file4 << duration.count() << " ";

        for (const auto& elem : cycles_cand.second) {
            cycle_file4 << elem << " ";
        }

        cycle_file4 << std::endl;
        cycle_file4.close();

        std::cout << "Cycle exported" << std::endl;
    } else {
        std::cerr << "Unable to open for export." << std::endl;
    }


    start = std::chrono::high_resolution_clock::now();
    cycles_cache = local_optim::cache_algorithm(cycles_random.first, cycles_random.second, dist_mat);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    total_length = Utils::calculate_cycle_length(dist_mat, cycles_cache.first) + Utils::calculate_cycle_length(dist_mat, cycles_cache.second);

    directoryPath = "output/cache_moves";
    fileName = instance + ".txt";

    filePath = fs::path(directoryPath) / fileName;

    if (!fs::exists(directoryPath)) {
        fs::create_directories(filePath.parent_path());
    }

    std::ofstream cycle_file5(filePath, std::ios::app);

    if (cycle_file5.is_open()) {

        cycle_file5 << run_nr << ": ";
        cycle_file5 << total_length << " ";
        cycle_file5 << duration.count() << " ";

        for (const auto& elem : cycles_cache.first) {
            cycle_file5 << elem << " ";
        }

        cycle_file5 << std::endl;

        cycle_file5 << run_nr << ": ";
        cycle_file5 << total_length << " ";
        cycle_file5 << duration.count() << " ";

        for (const auto& elem : cycles_cache.second) {
            cycle_file5 << elem << " ";
        }

        cycle_file5 << std::endl;
        cycle_file5.close();

        std::cout << "Cycle exported" << std::endl;
    } else {
        std::cerr << "Unable to open for export." << std::endl;
    }

}
