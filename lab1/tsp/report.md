# Sprawozdanie z laboratorium Inteligentne Metody Optymalizacji

## Krótki opis zadania.
Celem zadania było zaimplementowanie trzech heurystyk rozwiązujących problem komiwojazera, a następnie porównanie ich rozwiązań.

## Opis wszystkich zaimplementowanych algorytmów w pseudokodzie

### Heurystyka najbliższego sąsiada (nearest neighbour)

```pseudocode
Funkcja nearest_neighbour(dist_mat):
    dim := Rozmiar(dist_mat)
    cykl1 := Pusta lista
    cykl2 := Pusta lista
    odwiedzone := Tablica o rozmiarze dim, zainicjowana wartościami false

    losowy_indeks := Wybierz_randomowy_wierzcholek(0, dim, -1)
    cykl1.dodaj_element(losowy_indeks)
    odwiedzone[losowy_indeks] := true

    najblizszy_sasiad := Wybierz_najblizszego_sasiada(dist_mat, losowy_indeks, odwiedzone)
    cykl1.dodaj_element(najblizszy_sasiad)
    odwiedzone[najblizszy_sasiad] := true

    losowy_indeks2 := Wybierz_randomowy_wierzcholek(0, dim, losowy_indeks)
    cykl2.dodaj_element(losowy_indeks2)
    odwiedzone[losowy_indeks2] := true

    najblizszy_sasiad2 := Wybierz_najblizszego_sasiada(dist_mat, losowy_indeks2, odwiedzone)
    cykl2.dodaj_element(najblizszy_sasiad2)
    odwiedzone[najblizszy_sasiad2] := true

    Dopóki nie odwiedzono wszystkich wierzchołków:
        min_odleglosc := MAX_INT
        index_min_odleglosci := -1
        wierzcholek_min_odleglosci := -1

        Dla każdego wierzchołka i w dist_mat:
            Jeżeli i nie jest odwiedzony i odległość od bieżącego wierzchołka jest mniejsza niż min_odleglosc:
                min_odleglosc := odległość od bieżącego wierzchołka
                index_min_odleglosci := i
                wierzcholek_min_odleglosci := wierzchołek

        Dodaj wierzchołek_min_odleglosci do cyklu 1
        odwiedzone[wierzcholek_min_odleglosci] := true

        Zresetuj min_odleglosc, index_min_odleglosci i wierzcholek_min_odleglosci

        Dla każdego wierzchołka i w dist_mat:
            Jeżeli i nie jest odwiedzony i odległość od bieżącego wierzchołka jest mniejsza niż min_odleglosc:
                min_odleglosc := odległość od bieżącego wierzchołka
                index_min_odleglosci := i
                wierzcholek_min_odleglosci := wierzchołek

        Dodaj wierzchołek_min_odleglosci do cyklu 2
        odwiedzone[wierzcholek_min_odleglosci] := true

    Zwiększ każdy element w cyklu o 1, aby dopasować identyfikator wierzchołka

    Zwróć parę (cykl1, cykl2)
```

### Metoda rozbudowy cyklu (greedy cycle)

```pseudocode
Funkcja greedy_cycle(dist_mat):
    dim := Rozmiar(dist_mat)
    cykl1 := Pusta lista
    cykl2 := Pusta lista
    odwiedzone := Tablica o rozmiarze dim, zainicjowana wartościami false

    losowy_indeks := Wybierz_randomowy_wierzcholek(0, dim, -1)
    najblizszy_sasiad := Wybierz_najblizszego_sasiada(dist_mat, losowy_indeks, odwiedzone)

    cykl1.dodaj_element(losowy_indeks)
    cykl1.dodaj_element(najblizszy_sasiad)
    odwiedzone[losowy_indeks] := true
    odwiedzone[najblizszy_sasiad] := true

    losowy_indeks2 := Wybierz_randomowy_wierzcholek(0, dim, losowy_indeks)
    najblizszy_sasiad2 := Wybierz_najblizszego_sasiada(dist_mat, losowy_indeks2, odwiedzone)

    cykl2.dodaj_element(losowy_indeks2)
    cykl2.dodaj_element(najblizszy_sasiad2)
    odwiedzone[losowy_indeks2] := true
    odwiedzone[najblizszy_sasiad2] := true

    Dopóki nie odwiedzono wszystkich wierzchołków:
        min_odleglosc := MAX_INT
        index_min_odleglosci := -1
        wierzcholek_min_odleglosci := -1

        Dla każdego wierzchołka i w dist_mat:
            Jeżeli i nie jest odwiedzony:
                Dla każdej krawędzi w cyklu 1:
                    Oblicz odległość, gdyby i został wstawiony w tej pozycji
                    Jeżeli jest mniejsza niż min_odleglosc:
                        Zaktualizuj min_odleglosc, index_min_odleglosci i wierzcholek_min_odleglosci

        Dodaj wierzcholek_min_odleglosci do cyklu 1
        odwiedzone[wierzcholek_min_odleglosci] := true

        Zresetuj min_odleglosc, index_min_odleglosci i wierzcholek_min_odleglosci

        Dla każdego wierzchołka i w dist_mat:
            Jeżeli i nie jest odwiedzony:
                Dla każdej krawędzi w cyklu 2:
                    Oblicz odległość, gdyby i został wstawiony w tej pozycji
                    Jeżeli jest mniejsza niż min_odleglosc:
                        Zaktualizuj min_odleglosc, index_min_odleglosci i wierzcholek_min_odleglosci

        Dodaj wierzcholek_min_odleglosci do cyklu 2
        odwiedzone[wierzcholek_min_odleglosci] := true

    Zwiększ każdy element w cyklu o 1, aby dopasować identyfikator wierzchołka

    Zwróć parę (cykl1, cykl2)
```

### Heurystyki zachłanne oparte na żalu (regret heuristics)

```pseudocode
Funkcja regret_heuristic(distance_matrix, k=2):
    dim := Rozmiar(distance_matrix)
    start1 := Wybierz_randomowy_wierzcholek(0, dim, -1)
    start2 := Wybierz_randomowy_wierzcholek(0, dim, start1)

    unvisited_cities := Zbiór wszystkich miast poza start1 i start2

    cykl1 := [start1, start1]
    total_length1 := 0

    cykl2 := [start2, start2]
    total_length2 := 0

    Dopóki cykl1 nie ma rozmiaru dim/2:
        regrets := Pusta lista
        Dla każdego miasta w unvisited_cities:
            insertion_data := Pusta lista
            Dla każdej pozycji i w cyklu1:
                Oblicz długość cyklu po wstawieniu miasta w pozycję i
                Dodaj parę (długość, i) do insertion_data
            Posortuj insertion_data rosnąco według długości
            Jeżeli insertion_data ma co najmniej k elementów:
                regret := długość ostatniego elementu - długość pierwszego elementu
                best_length := długość pierwszego elementu
                best_i := pozycja pierwszego elementu
            W przeciwnym razie:
                regret := -długość pierwszego elementu
                best_length := długość pierwszego elementu
                best_i := pozycja pierwszego elementu
            Dodaj (regret, miasto, best_i, best_length) do regrets
        max_regret := Maksymalny element z regrets
        best_city := miasto z max_regret
        best_i := pozycja z max_regret
        total_length1 := długość cyklu po wstawieniu best_city w pozycję best_i
        Dodaj best_city do cyklu1
        Usuń best_city z unvisited_cities

    Dopóki unvisited_cities nie jest pusty:
        regrets := Pusta lista
        Dla każdego miasta w unvisited_cities:
            insertion_data := Pusta lista
            Dla każdej pozycji i w cyklu2:
                Oblicz długość cyklu po wstawieniu miasta w pozycję i
                Dodaj parę (długość, i) do insertion_data
            Posortuj insertion_data rosnąco według długości
            Jeżeli insertion_data ma co najmniej k elementów:
                regret := długość ostatniego elementu - długość pierwszego elementu
                best_length := długość pierwszego elementu
                best_i := pozycja pierwszego elementu
            W przeciwnym razie:
                regret := -długość pierwszego elementu
                best_length := długość pierwszego elementu
                best_i := pozycja pierwszego elementu
            Dodaj (regret, miasto, best_i, best_length) do regrets
        max_regret := Maksymalny element z regrets
        best_city := miasto z max_regret
        best_i := pozycja z max_regret
        total_length2 := długość cyklu po wstawieniu best_city w pozycję best_i
        Dodaj best_city do cyklu2
        Usuń best_city z unvisited_cities

    Zwiększ każdy element w cyklach o 1, aby dopasować identyfikator wierzchołka

    Zwróć parę (cykl1, cykl2)
```

## Wyniki Eksperymentu Obliczeniowego

| Instancja | Metoda            | Średnia (min – max)           |
|-----------|-------------------|-------------------------------|
| kroA100   | Greedy Cycle      | 1661.9    (1505 – 2279)       |
| kroA100   | Nearest Neighbour | 7428.58   (6909 – 21053)      |
| kroA100   | Regret Heuristic  | 35047.51  (32536 – 41120)     |
| kroB100   | Greedy Cycle      | 1668.07   (1503 – 2477)       |
| kroB100   | Nearest Neighbour | 7293.5    (6899 – 12841)      |
| kroB100   | Regret Heuristic  | 34894.93  (32749 – 46418)     |


## Wizualizacje Najlepszych Rozwiązań

| Instance  | Greedy Cycle | Nearest Neighbour | Regret Heuristic |
|-----------|--------------|-------------------|------------------|
| kroB100   | ![Greedy Cycle kroB100](photos/kroB100.tsp_greedy_cycle_best_visualization.png) | ![Nearest Neighbour kroB100](photos/kroB100.tsp_nearest_neighbour_best_visualization.png) | ![Regret Heuristic kroB100](photos/kroB100.tsp_regret_heuristic_best_visualization.png) |
| kroA100   | ![Greedy Cycle kroA100](photos/kroA100.tsp_greedy_cycle_best_visualization.png) | ![Nearest Neighbour kroA100](photos/kroA100.tsp_nearest_neighbour_best_visualization.png) | ![Regret Heuristic kroA100](photos/kroA100.tsp_regret_heuristic_best_visualization.png) |



## Wnioski

- Przy odpowiednio duzej liczbie uruchomień algorytmów, najlepsze rozwiązanie stanowi ten sam cykl dla wszystkich algorytmów
- Wraz ze wzrostem instancji problemu czas potrzebny do jego rozwiązania wzrasta około ośmiokrotnie

## Kod Programu

Kod programu znajduje się pod [tym linkiem](link_do_repozytorium).
