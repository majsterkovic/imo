#include <iostream>
#include <vector>

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

void local_search_steepest() {

}

void local_search_greedy(std::vector<int> starting_cycle) {

    // Wygeneruj wszystkie możliwe wymiany wierzchołków i wymiany krawędzi
    // Oceń ruch, jeżeli jest lepszy niż stan aktualny, to go zastosuj

}


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;

}
