#include <stdint.h>
#include <iostream>

#include "./gameobjects/camera.h"

#define N 4
#define M 4

int main(int argc, char **argv)
{
    Matrix matrix(N, M);
    Matrix matrix2(N, M);

    for (int i = 0; i < M; i++)
    {
        matrix.setValue(i, i, 1);
        matrix2.setValue(i, i, 1);
    }

    Matrix matrix3 = matrix * matrix2;

    matrix3.print();

    return 0;
}
