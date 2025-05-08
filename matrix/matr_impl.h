#ifndef MATR_IMPL_H
#define MATR_IMPL_H

#include "matrix.h"

template <typename T>
Matrix<T>::Matrix(int rows, int cols) : rows(rows), cols(cols) {
    matrix.resize(rows, std::vector<T>(cols, T()));
}

template <typename T>
Matrix<T>::~Matrix() = default;

template <typename T>
std::vector<T>& Matrix<T>::operator[](int index) {
    return matrix[index];
}

template <typename T>
const std::vector<T>& Matrix<T>::operator[](int index) const {
    return matrix[index];
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T> &other) {
    if (this != &other) {
        for (int i = 0; i < rows; ++i) {
            matrix[i].clear();
        }
        matrix.clear();
        rows = other.rows;
        cols = other.cols;
        matrix.resize(rows, std::vector<T>(cols));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                matrix[i][j] = other.matrix[i][j];
            }
        }
    }
    return *this;
}
#endif
