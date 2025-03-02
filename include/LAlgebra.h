//
// Created by zhang on 2025/2/28.
//

#ifndef LALGEBRA_H
#define LALGEBRA_H
#include <string>
#include <vector>

#define EIGEN_GROUP std::pair<std::vector<double>, std::vector<std::vector<double>>>

#define BLOCK_SIZE 32

class Matrix {
public:

    Matrix(int _row, int _column, std::vector<std::vector<double>> _values);

    int getRow() const;
    int getColumn() const;

    Matrix T();
    double determinant();
    Matrix inverse();
    EIGEN_GROUP eigen_vs();

    void print() const;

    Matrix operator + (const Matrix& other);
    Matrix operator - (const Matrix& other);
    Matrix operator * (const Matrix& other);
    Matrix operator * (double scalar);

private:
    int row, column;
    std::vector<std::vector<double>> values;


};

#endif //LALGEBRA_H
