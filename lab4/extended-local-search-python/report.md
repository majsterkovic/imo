## Sprawozdanie z laboratorium nr 4
# Inteligentne Metody Optymalizacji

Autorzy: Jakub Gołąb, Mariusz Hybiak

### Wprowadzenie
Celem zadania było rozszerzenie implementacji lokalnego przeszukiwania stosując trzy metody:
- (MSLS) Multiple start local search
- (ILS1) Iterated local search - Iteracyjne przeszukiwanie lokalne z niewielką perturbacją
- (ILS2) Iterated local search - Iteracyjne przeszukiwanie lokalne z Large-scale neighborhood search, tj. większą perturbacją typu Destroy-Repair
### Algorytmy
#### MSLS
```pseudocode

function MSLS(distance_matrix, data, n_iter):
    best_cycle1 = NULL
    best_cycle2 = NULL
    best_total_length = INFINITY

    for n_iter:
      cycle1, cycle2 = random_cycle(data)
      cycle1, cycle2 = candidate_moves_algorithm(cycle1, cycle2, distance_matrix, data)
      length = calculate_cycles_length(cycle1, cycle2, distance_matrix)

      if length < best_length:
        best_cycle_1 = cycle1
        best_cycle_2 = cycle2
        best_length = length
    
    return best_cycle1, best_cycle2
```
#### ILS1
```pseudocode
function ILS1(distance_matrix, data, time_MSLS):
    best_cycle_1, best_cycle_2 = random_cycle(data)
    best_cycle_1, best_cycle_2 = candidate_moves_algorithm(best_cycle_1, best_cycle_2, distance_matrix, data)
    best_length = calculate_cycles_length(best_cycle_1, best_cycle_2, distance_matrix)

    while time_elapsed <= time_MSLS:
        cycle1, cycle2 = little_perturbation(best_cycle_1, best_cycle_2)
        cycle1, cycle2 = candidate_moves_algorithm(cycle1, cycle2, distance_matrix, data)
        length = calculate_cycles_length(cycle1, cycle2, distance_matrix)
        if length < best_length:
            best_length = length
            best_cycle_1 = cycle1
            best_cycle_2 = cycle2
    
    return best_cycle_1, best_cycle_2

function little_perturbation(cycle1, cycle2):
    foreach cycle in set_of[cycle1, cycle2]:
        cycle = swap_random_nodes(cycle1, cycle2, 0.2)

    return cycle1, cycle2
```

### ILS2
```
function ILS2(distance_matrix, data, time_MSLS):
    best_cycle_1, best_cycle_2 = random_cycle(data)
    best_cycle_1, best_cycle_2 = candidate_moves_algorithm(best_cycle_1, best_cycle_2, distance_matrix, data)
    best_length = calculate_cycles_length(best_cycle_1, best_cycle_2, distance_matrix)

    while time_elapsed <= time_MSLS:
        cycle1, cycle2 = severe_perturbation(best_cycle_1, best_cycle_2)
        cycle1, cycle2 = candidate_moves_algorithm(cycle1, cycle2, distance_matrix, data)
        length = calculate_cycles_length(cycle1, cycle2, distance_matrix)
        if length < best_length:
            best_length = length
            best_cycle_1 = cycle1
            best_cycle_2 = cycle2
    
    return best_cycle_1, best_cycle_2

function severe_perturbation(cycle1, cycle2, distance_matrix):
    original_size_cycle1 = length_of(cycle1)
    original_size_cycle2 = length_of(cycle2)

    random1 = get_random_nodes_of(cycle1)
    random2 = get_random_nodes_of(cycle2)

    cycle1 = swap_random_nodes_of(cycle1, random1)
    cycle2 = swap_random_nodes_of(cycle2, random2)

    # Repair
    cities = random1 + random2

    while length_of(cycle1) < original_size_cycle1:
        current_city = cycle1.last_element()
        nearest_city = find_closest_city(current_city, distance_matrix)
        cycle1.append(nearest_city)
        cities.remove(nearest_city)

    while length_of(cycle2) < original_size_cycle2:
        current_city = cycle2.last_element()
        nearest_city = find_closest_city(current_city, distance_matrix)
        cycle2.append(nearest_city)
        cities.remove(nearest_city)
    
    return cycle1, cycle2
```
### Wyniki eksperymentu obliczeniowego
W tabeli przedstawiono sumy długości cykli dla każdej z metod dla obu instancji problemu.

<div>
<table border="1" class="dataframe">
  <thead>
    <tr>
      <th></th>
      <th></th>
      <th colspan="3" halign="left">Długość cyklu</th>
    </tr>
    <tr>
      <th></th>
      <th></th>
      <th>min</th>
      <th>mean</th>
      <th>max</th>
    </tr>
    <tr>
      <th>Instancja</th>
      <th>Algorytm</th>
      <th></th>
      <th></th>
      <th></th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th rowspan="3" valign="top">kroA200.tsp</th>
      <th>MSLS</th>
      <td>43440.074</td>
      <td>44181.734</td>
      <td>44552.210</td>
    </tr>
    <tr>
      <th>ISL1</th>
      <td>47933.072</td>
      <td>48803.432</td>
      <td>50472.015</td>
    </tr>
    <tr>
      <th>ISL2</th>
      <td>31617.228</td>
      <td>34258.148</td>
      <td>39236.820</td>
    </tr>
    <tr>
      <th rowspan="3" valign="top">kroB200.tsp</th>
      <th>MSLS</th>
      <td>0</td>
      <td>0</td>
      <td>0</td>
    </tr>
    <tr>
      <th>ISL1</th>
      <td>0</td>
      <td>0</td>
      <td>0</td>
    </tr>
    <tr>
      <th>ISL2</th>
      <td>0</td>
      <td>0</td>
      <td>0</td>
    </tr>
  </tbody>
</table>
</div>

### Czas działania algorytmu
W tabeli przedstawiono średni czas działania algorytmu.

<div>
<table border="1" class="dataframe">
  <thead>
    <tr>
      <th></th>
      <th></th>
      <th colspan="3" halign="left">Czas wykonania</th>
    </tr>
    <tr>
      <th></th>
      <th></th>
      <th>min</th>
      <th>mean</th>
      <th>max</th>
    </tr>
    <tr>
      <th>Instancja</th>
      <th>Algorytm</th>
      <th></th>
      <th></th>
      <th></th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th rowspan="3" valign="top">kroA200.tsp</th>
      <th>MSLS</th>
      <td>593.978</td>
      <td>595.418</td>
      <td>596.176</td>
    </tr>
    <tr>
      <th>ISL1</th>
      <td>600.318</td>
      <td>601.727</td>
      <td>602.610</td>
    </tr>
    <tr>
      <th>ISL2</th>
      <td>600.034</td>
      <td>602.170</td>
      <td>603.832</td>
    </tr>
    <tr>
      <th rowspan="3" valign="top">kroB200.tsp</th>
      <th>MSLS</th>
      <td>0</td>
      <td>0</td>
      <td>0</td>
    </tr>
    <tr>
      <th>ISL1</th>
      <td>0</td>
      <td>0</td>
      <td>0</td>
    </tr>
    <tr>
      <th>ISL2</th>
      <td>0</td>
      <td>0</td>
      <td>0</td>
    </tr>
  </tbody>
</table>
</div>


### Wizualizacje najlepszych rozwiązań
| Metoda |  KroA200 |   KroB200 |
|--------|----------|-----------|
| MSLS | ![](msls_kroa200.png) | ![](msls_krob200.png) |
| ISL1 (greedy cycle) | ![](isl1_kroa200.png) | ![](isl1_krob200.png) |
| ISL2 | ![](isl2_kroa200.png) | ![](isl2_krob200.png) |


### Wnioski
- Wnioski do napisania.

### Kod programu

Kod programu znajduje się pod [tym linkiem](https://github.com/majsterkovic/imo/blob/main/lab3/).