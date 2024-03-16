#include "./vectors.h"

/* constructors */
Vector3D::Vector3D()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
}

Vector3D::Vector3D(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
