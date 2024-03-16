#ifndef VECTORS_H
#define VECTORS_H

#include "matrix.h"

class Vector3D
{
private:
    float x, y, z;

public:
    Vector3D(float x, float y, float z);
    Vector3D();

    float getX() { return x; };
    float getY() { return y; };
    float getZ() { return z; };

    void setX(float value) { x = value; };
    void setY(float value) { y = value; };
    void setZ(float value) { z = value; };
};

#endif /* VECTORS_H */

#ifndef QUATERNION_H
#define QUATERNION_H

class Quaternion
{
private:
    float a, b, c, d;

public:
    Quaternion();

    Matrix as_matrix();
};

#endif /* QUATERNION_H */