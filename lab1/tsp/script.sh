#!/bin/bash

input_files=("kroB100.tsp" "kroA100.tsp")
algorithms=("nearest_neighbour" "greedy_cycle" "regret_heuristic")

rm tsp_executable
g++ -o tsp_executable main.cpp



for algorithm in "${algorithms[@]}"
do
    rm time_"$algorithm"_"$input_file".txt

    for input_file in "${input_files[@]}"
    do
        rm cycle_"$algorithm"_"$input_file".txt
        for i in {1..100}
        do
            ./tsp_executable "$input_file" "$algorithm" "${i}"
        done
    done
done