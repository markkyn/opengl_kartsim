// matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include <cstddef>

class Matrix
{
private:
    float **data;
    size_t n, m;

public:
    Matrix(size_t n, size_t m);

    void setValue(int x, int y, float value) { data[x][y] = value; };
    float getValue(int x, int y) { return data[x][y]; };

    size_t get_n() { return n; };
    size_t get_m() { return m; };

    void set_n(size_t new_n) { n = new_n; };
    void set_m(size_t new_m) { m = new_m; };

    Matrix operator+(Matrix const &dir) const;
    Matrix operator-(Matrix const &dir) const;
    Matrix operator*(Matrix const &dir) const; // mult
    Matrix operator,(Matrix const &dir) const; // dot
    Matrix operator/(Matrix const &dir) const;

    void print();
};

#endif /* MATRIX_H */