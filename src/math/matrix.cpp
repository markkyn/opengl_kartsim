// matrix.cpp

#include <iostream>
#include "matrix.h"

/*
  o ----------------------> y
  | (0,0) (0,1) ...    (0, m-1)
  | (1,0) (1,1) ...    (1, m-1)
  |  ...        ...    ...
  |  ...        ...    ...
  |  (n-1, 0)   ...  (n-1, m-1)
 \|/
  x
*/

Matrix::Matrix(size_t n, size_t m)
{
  Matrix::n = n;
  Matrix::m = m;

  // Declaration
  data = new float *[n];

  for (int i = 0; i < m; i++)
    data[i] = new float[m];

  // Setting all zeros
  for (int x = 0; x < n; x++)
    for (int y = 0; y < m; y++)
      data[x][y] = 0.0;
}

size_t Matrix::get_n()
{
  return Matrix::n;
}

void Matrix::set_n(size_t n)
{
  size_t curr_n = Matrix::n;

  for (int i = curr_n; i < n; i++)
    data[i] = new float[Matrix::get_m()];
}

size_t Matrix::get_m()
{
  return Matrix::m;
}

void Matrix::set_m(size_t m)
{
  size_t curr_m = Matrix::m;
}

Matrix Matrix::operator+(Matrix const &matrix2)
{
  for (int i = 0; i < Matrix::n; i++)
    for (int j = 0; j < Matrix::m; j++)
      Matrix::data[i][j] += matrix2.data[i][j];
}

void Matrix::print()
{
  for (int i = 0; i < Matrix::n; i++)
    for (int j = 0; j < Matrix::m; j++)
      
}