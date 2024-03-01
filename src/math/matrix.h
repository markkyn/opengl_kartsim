// matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>

class Matrix
{
private:
    float **data;
    size_t n, m;

public:
    Matrix(size_t n, size_t m);

    size_t get_n();
    void set_n(size_t n);

    size_t get_m();
    void set_m(size_t m);

    float get_data(int32_t x, int32_t y);

    Matrix operator+(Matrix const &dir);
    Matrix operator-(Matrix const &dir);
    Matrix operator*(Matrix const &dir); // mult
    Matrix operator,(Matrix const &dir); // dot
    Matrix operator/(Matrix const &dir);

    void print();
};

#endif /* MATRIX_H */