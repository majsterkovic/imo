#!/bin/bash


# Usuń poprzedni katalog build i utwórz go na nowo
rm -rf build
rm -rf output

mkdir build
cd build || exit


# Konfiguracja CMake i kompilacja
cmake ..
make

# Sprawdź, czy program został pomyślnie zbudowany
if [ ! -f "local_search" ]; then
    echo "Compilation failed."
    exit 1
fi

# Tablice z parametrami
instances=("kroA100" "kroB100")
methods=("steepest" "greedy")
neighbourhoods=("inner" "between")
beginnings=("random" "heuristic")

# Uruchom program w pętli dla każdej kombinacji parametrów
for instance in "${instances[@]}"; do
    for method in "${methods[@]}"; do
        for neighbourhood in "${neighbourhoods[@]}"; do
            for beginning in "${beginnings[@]}"; do
                for (( run=1; run<=100; run++ )); do
                    ./local_search "$instance" "$method" "$neighbourhood" "$beginning" "$run"
                done
            done
        done
    done
done
