//
// Created by Mariusz on 21.03.2024.
//

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <stdexcept>

class Matrix {
private:
    std::vector<std::vector<int>> data;

public:
    Matrix(int rows, int cols) : data(rows, std::vector<int>(cols)) {}

    std::vector<int>& operator[](int row) {
        if (row >= data.size()) {
            throw std::out_of_range("Matrix indices out of range");
        }
        return data[row];
    }

    const std::vector<int>& operator[](int row) const {
        if (row >= data.size()) {
            throw std::out_of_range("Matrix indices out of range");
        }
        return data[row];
    }

    int getRows() const {
        return data.size();
    }

    int getCols() const {
        return data[0].size();
    }
};



#endif //MATRIX_H
