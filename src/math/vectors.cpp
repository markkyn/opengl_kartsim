#include <cmath> 
#include <algorithm>

#include "./vectors.h"

/* ructors */
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

Vector3D Vector3D::operator+(Vector3D &v)
{
    return Vector3D(x + v.x, y + v.y, z + v.z);
}

Vector3D Vector3D::operator-(Vector3D &v)
{
    return Vector3D(x - v.x, y - v.y, z - v.z);
}

Vector3D Vector3D::operator*(float scalar)
{
    return Vector3D(x * scalar, y * scalar, z * scalar);
}

float Vector3D::dot(Vector3D &v)
{
    return x * v.x + y * v.y + z * v.z;
}

Vector3D Vector3D::cross(Vector3D &v)
{
    return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

float Vector3D::magnitude()
{
    return sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::normalize()
{
    float mag = magnitude();
    return Vector3D(x / mag, y / mag, z / mag);
}

void Vector3D::rotate(float angleX, float angleY, float angleZ)
{
    // X Rotation
    Matrix rotationX(3, 3);
    rotationX.setValue(0, 0, 1);
    rotationX.setValue(1, 1, cos(angleX));
    rotationX.setValue(2, 2, cos(angleX));
    rotationX.setValue(1, 2, -sin(angleX));
    rotationX.setValue(2, 1, sin(angleX));

    // Y Rotation
    Matrix rotationY(3, 3);
    rotationY.setValue(0, 0, cos(angleY));
    rotationY.setValue(0, 2, sin(angleY));
    rotationY.setValue(1, 1, 1);
    rotationY.setValue(2, 0, -sin(angleY));
    rotationY.setValue(2, 2, cos(angleY));

    // Z Rotation
    Matrix rotationZ(3, 3);
    rotationZ.setValue(0, 0, cos(angleZ));
    rotationZ.setValue(0, 1, -sin(angleZ));
    rotationZ.setValue(1, 0, sin(angleZ));
    rotationZ.setValue(1, 1, cos(angleZ));
    rotationZ.setValue(2, 2, 1);

    Matrix rotationMatrix = rotationZ * rotationY * rotationX;

    Matrix vectorAsMatrix = this->toMatrix(); // [ x, y, z ]

    Matrix rotatedMatrix = rotationMatrix * vectorAsMatrix;

    this->x = rotatedMatrix.getValue(0, 0);
    this->y = rotatedMatrix.getValue(1, 0);
    this->z = rotatedMatrix.getValue(2, 0);
}

Matrix Vector3D::toMatrix()
{
    Matrix matrix(3, 1);

    matrix.setValue(0, 0, this->x); // x
    matrix.setValue(1, 0, this->y); // y
    matrix.setValue(2, 0, this->z); // z

    return matrix;
}

float Vector3D::angleBetween(Vector3D vector)
{
    float dotProd = this->dot(vector);
    float magA = this->magnitude(); 
    float magB = vector.magnitude();

    float cosTheta = dotProd / (magA * magB);

    cosTheta = std::max(-1.0f, std::min(1.0f, cosTheta));

    return acos(cosTheta);
}