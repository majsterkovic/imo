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
instances=("kroA200" "kroB200")
methods=("steepest" "greedy")
neighbourhoods=("inner" "between")
beginnings=("random" "heuristic")

for instance in "${instances[@]}"; do
  for (( run=1; run<=3; run++ )); do
    ./local_search "$instance" "random" "inner" "random" "$run"
  done
done
