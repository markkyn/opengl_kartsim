#ifndef VECTORS_H
#define VECTORS_H

#include <math.h>
#include <cmath> 

#include "matrix.h"

typedef struct
{
    double angleX;
    double angleY;
    double angleZ;
} Angles;

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

    /* Operators */
    Vector3D operator+(Vector3D &v);
    Vector3D operator-(Vector3D &v);
    Vector3D operator*(float scalar);
    float dot(Vector3D &v);
    Vector3D cross(Vector3D &v);
    void rotate(float angleX, float angleY, float angleZ);

    float magnitude();
    Vector3D normalize();

    Matrix toMatrix();
    float angleBetween(Vector3D vector);
};

#endif /* VECTORS_H */

#ifndef QUATERNION_H
#define QUATERNION_H

struct Quaternion
{
    double w, x, y, z;

    Quaternion(double angle, double ux, double uy, double uz)
    {
        double rad = angle * M_PI / 180.0;
        w = cos(rad / 2);
        x = ux * sin(rad / 2);
        y = uy * sin(rad / 2);
        z = uz * sin(rad / 2);
    }

    Quaternion operator*(const Quaternion &q) const
    {
        return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z, // Parte real
            w * q.x + x * q.w + y * q.z - z * q.y, // i componente
            w * q.y - x * q.z + y * q.w + z * q.x, // j componente
            w * q.z + x * q.y - y * q.x + z * q.w  // k componente
        );
    }

    Quaternion conjugate() const
    {
        return Quaternion(w, -x, -y, -z);
    }
};

#endif /* QUATERNION_H */