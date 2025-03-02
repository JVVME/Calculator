//
// Created by zhang on 2025/2/28.
//

#include "include/LAlgebra.h"

#include <iostream>

Matrix::Matrix(int _row, int _column, std::vector<std::vector<double>> _values):
row(_row), column(_column), values(std::move(_values)) {
}

Matrix Matrix::T() {
    std::vector<std::vector<double>> transpose(column, std::vector<double>(row, 0.0));
    for (int i = 0; i < column; i++) {
        for (int j = 0; j < row; j++) {
            transpose[i][j] = values[j][i];
        }
    }


    return {column, row, transpose};
}

void Matrix::print() const {
    std::cout << "(" << row << "*" << column << ")" << std::endl;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            std::cout << values[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int Matrix::getRow() const {
    return row;
}

int Matrix::getColumn() const {
    return column;
}

double Matrix::determinant() {
    if (row != column) {
        std::cout << "Matrix width not equal to height, invalid.";
        return -1;
    }
    int n = values.size();
    if (n == 0 || values[0].size() != n) {
        throw std::invalid_argument("Matrix must be square");
    }

    std::vector<std::vector<double>> LU = values; // 复制矩阵
    int swaps = 0; // 记录主元交换次数

    for (int i = 0; i < n; i++) {
        // 寻找主元
        double maxElement = std::abs(LU[i][i]);
        int maxRow = i;
        for (int k = i + 1; k < n; k++) {
            if (std::abs(LU[k][i]) > maxElement) {
                maxElement = std::abs(LU[k][i]);
                maxRow = k;
            }
        }

        // 交换行
        if (maxRow != i) {
            std::swap(LU[i], LU[maxRow]);
            swaps++;
        }

        // 高斯消元
        for (int k = i + 1; k < n; k++) {
            if (LU[i][i] == 0) {
                throw std::runtime_error("Matrix is singular");
            }
            double factor = LU[k][i] / LU[i][i];
            for (int j = i; j < n; j++) {
                LU[k][j] -= factor * LU[i][j];
            }
        }
    }

    // 计算行列式
    double det = (swaps % 2 == 0 ? 1 : -1);
    for (int i = 0; i < n; i++) {
        det *= LU[i][i];
    }
    return det;
}



Matrix Matrix::inverse() {

    if(row == 0 || values[0].size() != row) {
        throw std::invalid_argument("Matrix must be square.");
    }

    std::vector<std::vector<double>> augmented_matrix(row, std::vector<double>(2*row, 0));
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < row; j++) {
            augmented_matrix[i][j] = values[i][j];
        }
        augmented_matrix[i][row + i] = 1;
    }

    const double epsilon = 1e-9;
    for(int j = 0; j < row; j++) {
        int current_row = j;
        double max_val = 0;
        int max_row = current_row;
        for(int i = current_row; i < row; i++) {
            if(std::abs(augmented_matrix[i][j]) > max_val) {
                max_val = std::abs(augmented_matrix[i][j]);
                max_row = i;
            }
        }
        if(max_val < epsilon) {
            throw std::runtime_error("Matrix is singular and cannot be inverted.");
        }

        std::swap(augmented_matrix[current_row], augmented_matrix[max_row]);
        double pivot = augmented_matrix[current_row][j];
        for(int k = 0; k < 2*row; k++) {
            augmented_matrix[current_row][k] /= pivot;
        }

        for(int i = 0; i < row; i++) {
            if(i != current_row) {
                double factor = augmented_matrix[i][j];
                for(int k = 0; k < 2*row; k++) {
                    augmented_matrix[i][k] -= factor * augmented_matrix[current_row][k];
                }
            }
        }
    }

    std::vector<std::vector<double>> res(row, std::vector<double>(row));
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < row; j++) {
            res[i][j] = augmented_matrix[i][row + j];
        }
    }
    return {row, column, res};
}

// std::pair<std::vector<double>, std::vector<std::vector<double>>> Matrix::eigen_vs() {
// }

Matrix Matrix::operator+(const Matrix &other) {
    if (other.getRow() != row || other.column != column) {
        std::cout << "Can't do \"+\"" << std::endl;
        return *this;
    }

    std::vector<std::vector<double>> res(row, std::vector<double>(column, 0));

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            res[i][j] = values[i][j] + other.values[i][j];
        }
    }

    return {row, column, res};
}

Matrix Matrix::operator-(const Matrix &other) {
    if (other.getRow() != row || other.column != column) {
        std::cout << "Can't do \"-\"" << std::endl;
        return *this;
    }

    std::vector<std::vector<double>> res(row, std::vector<double>(column, 0));

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            res[i][j] = values[i][j] - other.values[i][j];
        }
    }

    return {row, column, res};
}


std::vector<double> flatten(const std::vector<std::vector<double>>& mat) {
    std::vector<double> flat;
    for (const auto& row : mat) {
        flat.insert(flat.end(), row.begin(), row.end());
    }
    return flat;
}

std::vector<std::vector<double>> unflatten(const std::vector<double>& flat, int m, int p) {
    std::vector<std::vector<double>> mat(m, std::vector<double>(p));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            mat[i][j] = flat[i * p + j];
        }
    }
    return mat;
}

std::vector<double> multiply_1D(const std::vector<double>& A_flat, const std::vector<double>& B_flat, int m, int n, int p) {
    std::vector<double> C_flat(m * p, 0.0);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k += BLOCK_SIZE) {
                int k_end = std::min(k + BLOCK_SIZE, n);
                for (int kk = k; kk < k_end; kk++) {
                    sum += A_flat[i * n + kk] * B_flat[kk * p + j];
                }
            }
            C_flat[i * p + j] = sum;
        }
    }
    return C_flat;
}

std::vector<std::vector<double>> matrix_multiply(const std::vector<std::vector<double>>& A,
                                                  const std::vector<std::vector<double>>& B) {
    int m = A.size();
    if (m == 0) {
        throw std::invalid_argument("Matrix A is empty.");
    }
    int n = A[0].size();
    for (const auto& row : A) {
        if (row.size() != n) {
            throw std::invalid_argument("Matrix A has rows of different lengths.");
        }
    }
    int p = B[0].size();
    for (const auto& row : B) {
        if (row.size() != p) {
            throw std::invalid_argument("Matrix B has rows of different lengths.");
        }
    }
    if (n != B.size()) {
        throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
    }
    std::vector<double> A_flat = flatten(A);
    std::vector<double> B_flat = flatten(B);
    std::vector<double> C_flat = multiply_1D(A_flat, B_flat, m, n, p);
    return unflatten(C_flat, m, p);
}

Matrix Matrix::operator*(const Matrix &other) {
    return {row, other.column, matrix_multiply(values, other.values)};
}

Matrix Matrix::operator*(double scalar) {
    std::vector<std::vector<double>> res(row, std::vector<double>(column, 0));

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            res[i][j] = values[i][j] * scalar;
        }
    }

    return {row, column, res};
}

//matrix A=[[90,89,2,33],[29,42,77,7],[10,21,25,20]]
//matrix B=[[27, 68],[0, 84],[14,24],[43,47]]
//matrix A = [[1,2,3],[3,2,2],[4,5,9]]