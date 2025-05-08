#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

template <typename T>
class Matrix {
public:
    Matrix(int rows, int cols);

    ~Matrix();

    std::vector<T>& operator[](int index);
    const std::vector<T>& operator[](int index) const;
    Matrix<T>& operator=(const Matrix<T> &other);


private:
    int rows;
    int cols;
    std::vector<std::vector<T>> matrix;
};

#include "matr_impl.h"
#endif
