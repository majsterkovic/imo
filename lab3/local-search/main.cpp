

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
        instance = "kroB100";
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


    if (beginning == "random") {
        cycles = Utils::random_cycles(dist_mat);
    } else if (beginning == "heuristic") {
        cycles = greedy::nearest_neighbour(dist_mat);
    }


    //c = local_optim::candidate_moves_algorithm(cycles.first, cycles.second, dist_mat, 10);
    c = local_optim::cache_algorithm(cycles.first, cycles.second, dist_mat);

    int length_before = Utils::calculate_cycle_length(dist_mat, cycles.first) + Utils::calculate_cycle_length(dist_mat, cycles.second);
    int length_after = Utils::calculate_cycle_length(dist_mat, c.first) + Utils::calculate_cycle_length(dist_mat, c.second);

    std::cout << length_before << std::endl;
    std::cout << length_after << std::endl;



//
//    int length_before = Utils::calculate_cycle_length(dist_mat, cycles.first) + Utils::calculate_cycle_length(dist_mat, cycles.second);
//    int length_after = Utils::calculate_cycle_length(dist_mat, c.first) + Utils::calculate_cycle_length(dist_mat, c.second);
//
//    std::string directoryPath = "output/" + method + "/" + neighbourhood + "/" + beginning;
//    std::string fileName = instance + ".txt";
//
//    fs::path filePath = fs::path(directoryPath) / fileName;
//
//    if (!fs::exists(directoryPath)) {
//        fs::create_directories(filePath.parent_path());
//    }
//

//    std::ofstream cycle_file(filePath, std::ios::app);
//
//    if (cycle_file.is_open()) {
//
//        cycle_file << run_nr << ": ";
//        cycle_file << length_after << " ";
//
//        for (const auto& elem : c.first) {
//            cycle_file << elem << " ";
//        }
//
//        cycle_file << std::endl;
//
//        cycle_file << run_nr << ": ";
//        cycle_file << length_after << " ";
//
//        for (const auto& elem : c.second) {
//            cycle_file << elem << " ";
//        }
//
//        cycle_file << std::endl;
//        cycle_file.close();
//
//        std::cout << "Cycle exported" << std::endl;
//    } else {
//        std::cerr << "Unable to open for export." << std::endl;
//    }


}
