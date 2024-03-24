## Sprawozdanie z laboratorium nr 2
# Inteligentne Metody Optymalizacji

Autorzy: Jakub Gołąb, Mariusz Hybiak

### Wprowadzenie
Celem zadania była implementacja algorytmów lokalnego przeszukiwania w wersjach stromej (steepest) i zachłannej (greedy), z dwoma różnym rodzajami sąsiedztwa, starując albo z rozwiązań losowych, albo z rozwiązań uzyskanych za pomocą jednej z heurystyk opracowanych w ramach poprzedniego zadania.

### Algorytmy
```pseudocode
// Generowanie wszystkich możliwych ruchów w ramach pojedynczego cyklu
generate_intra_route_inner(cycle):
    moves = pusta lista
    cycle_size = rozmiar(cycle)

    // Generowanie ruchów odwracających podciągi
    dla i od 0 do cycle_size-1:
        dla j od i+1 do cycle_size:
            dodaj (REVERSE_SUBSEQUENCE, (i, j)) do moves

    // Generowanie ruchów zamieniających wierzchołki
    dla i od 2 do cycle_size-2:
        dla j od 0 do cycle_size:
            dodaj (SWAP_NODES, (j, (j+i) % cycle_size)) do moves

    // Losowe przemieszanie listy ruchów
    przemieszaj moves

    zwróć moves
```

```pseudocode
// Lokalne przeszukiwanie metodą stromą w ramach pojedynczego cyklu
local_search_steepest_inner_cycle(starting_cycle, dist_mat):
    moves = generate_intra_route_inner(starting_cycle)
    tmp_cycle = kopia starting_cycle
    best_index = -1
    best_value = 0

    while true:
        best_index = -1
        best_value = 0

        // Dla każdego ruchu z listy ruchów
        dla każdego i od 0 do rozmiar(moves)-1:
            move = moves[i]
            node1 = move.pierwszy_cykl
            node2 = move.drugi_cykl
            type = move.typ_ruchu

            // Obliczanie nowego cyklu i wartości zmiany
            jeżeli type == REVERSE_SUBSEQUENCE:
                tmp_cycle = odwróć_podciąg(starting_cycle, node1, node2)
                delta = oblicz_delte(starting_cycle, tmp_cycle)
            jeżeli type == SWAP_NODES:
                tmp_cycle = zamień_wierzchołki(starting_cycle, node1, node2)
                delta = oblicz_delte(starting_cycle, tmp_cycle)
            // Jeżeli wartość zmiany jest większa od najlepszej wartości dotychczas
            jeżeli delta > best_value:
                best_value = delta
                best_index = i

        // Jeżeli istnieje lepszy ruch, zastosuj go
        jeżeli best_value > 0:
            move = moves[best_index]
            zastosuj move
            del(moves[best_index])
        w przeciwnym razie:
            przerwij pętlę

    zwróć starting_cycle
```

```pseudocode
local_search_greedy_inner_cycle(starting_cycle, dist_mat):
    moves = generate_intra_route_inner(starting_cycle)
    tmp_cycle = kopia starting_cycle
    best_index = -1
    best_value = 0

    // Losowe przemieszanie listy ruchów
    przemieszaj moves

    // Dla każdego ruchu z listy ruchów
    dla każdego i od 0 do rozmiar(moves)-1:
        move = moves[i]
        node1 = move.drugi.pierwszy
        node2 = move.drugi.drugi
        type = move.pierwszy

        // Obliczanie nowego cyklu i wartości zmiany
        jeżeli type == REVERSE_SUBSEQUENCE:
            tmp_cycle = odwróć_podciąg(starting_cycle, node1, node2)
            delta = oblicz_delte(starting_cycle, tmp_cycle)
        jeżeli type == SWAP_NODES:
            tmp_cycle = zamień_wierzchołki(starting_cycle, node1, node2)
            delta = oblicz_delte(starting_cycle, tmp_cycle)
        // Jeżeli wartość zmiany jest większa od najlepszej wartości dotychczas
        jeżeli delta > best_value:
            best_value = delta
            best_index = i
            zastosuj ruch move

    zwróć starting_cycle
```

### Wyniki eksperymentu obliczeniowego

W tabeli przedstawiono sumy długości cykli dla każdej z metod dla obu instancji problemu.

| Instancja   | Metoda            | Średnia (min – max) [jednostki odległości]    |
|-------------|-------------------|-----------------------------|
| kroA100 | steepest inner random | 33851.17 (31816 - 36246) |
| kroA100 | steepest inner heuristic | 26812.36 (23904 - 30188) |
| kroA100 | steepest between random | 52910.15 (41042 - 67550) |
| kroA100 | steepest between heuristic | 31630.19 (28769 - 35170) |
| kroA100 | greedy inner random | 34417.8 (30801 - 38255) |
| kroA100 | greedy inner heuristic | 27258.43 (23463 - 29234) |
| kroA100 | greedy between random | 43123.97 (36266 - 54241) |
| kroA100 | greedy between heuristic | 31974.89 (29382 - 34758) |
| kroB100 | steepest inner random | 34262.46 (31783 - 36894) |
| kroB100 | steepest inner heuristic | 25954.37 (22611 - 29852) |
| kroB100 | steepest between random | 51914.82 (41791 - 65046) |
| kroB100 | steepest between heuristic | 31597.95 (28709 - 33634) |
| kroB100 | greedy inner random | 34650.79 (31962 - 37520) |
| kroB100 | greedy inner heuristic | 26111.22 (23123 - 30336) |
| kroB100 | greedy between random | 43749.95 (35753 - 56861) |
| kroB100 | greedy between heuristic | 31697.67 (28759 - 34261) |


### Wizualizacje najlepszych rozwiązań

| Metoda |  KroA100 |   KroB100 |
|--------|----------|-----------|
| steepest inner random | ![steepest_inner_random_A](local-search/plots/kroA100_steepest_inner_random.png) | ![steepest_inner_random_B](local-search/plots/kroB100_steepest_inner_random.png) |
| steepest inner heuristic | ![steepest_inner_heuristic_A](local-search/plots/kroA100_steepest_inner_heuristic.png) | ![steepest_inner_heuristic_B](local-search/plots/kroB100_steepest_inner_heuristic.png) |
| steepest between random | ![steepest_between_random_A](local-search/plots/kroA100_steepest_between_random.png) | ![steepest_between_random_B](local-search/plots/kroB100_steepest_between_random.png) |
| steepest between heuristic | ![steepest_between_heuristic_A](local-search/plots/kroA100_steepest_between_heuristic.png) | ![steepest_between_heuristic_B](local-search/plots/kroB100_steepest_between_heuristic.png) |
| greedy inner random | ![greedy_inner_random_A](local-search/plots/kroA100_greedy_inner_random.png) | ![greedy_inner_random_B](local-search/plots/kroB100_greedy_inner_random.png) |
| greedy inner heuristic | ![greedy_inner_heuristic_A](local-search/plots/kroA100_greedy_inner_heuristic.png) | ![greedy_inner_heuristic_B](local-search/plots/kroB100_greedy_inner_heuristic.png) |
| greedy between random | ![greedy_between_random_A](local-search/plots/kroA100_greedy_between_random.png) | ![greedy_between_random_B](local-search/plots/kroB100_greedy_between_random.png) |
| greedy between heuristic | ![greedy_between_heuristic_A](local-search/plots/kroA100_greedy_between_heuristic.png) | ![greedy_between_heuristic_B](local-search/plots/kroB100_greedy_between_heuristic.png) |


### Wnioski

- Algorytmy lokalnego przeszukiwania, zarówno w wersji stromej jak i zachłannej, wykazują różnice w skuteczności w zależności od wyboru początkowego rozwiązania oraz rodzaju sąsiedztwa.
- Wykorzystanie heurystyk w początkowych rozwiązaniach znacząco poprawia efektywność algorytmów, szczególnie widoczne jest to w przypadku metodfy stromej.
- Local search w wersji stromej osiąga zazwyczaj lepsze wyniki od metody zachłannej, co sugeruje, że bardziej dogłębne przeszukiwanie sąsiedztwa przynosi korzyści w przypadku tego konkretnego problemu.


### Kod Programu

Kod programu znajduje się pod [tym linkiem](https://github.com/majsterkovic/imo/blob/main/lab2/) w pliku cpp.
