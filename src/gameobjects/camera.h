#ifndef CAMERA_H
#define CAMERA_H

#include "../math/vectors.h"

class Camera
{
private:
    float x, y, z; // Global Position

    Vector3D lookingAt;
    Vector3D upVector;

public:
    Camera(float sX, float sY, float sZ);

    Vector3D getPosition() { return Vector3D(x, y, z); };

    /* Getters */
    float getX() { return x; };
    float getY() { return y; };
    float getZ() { return z; };

    /* Setters*/
    void setX(float value) { x = value; };
    void setY(float value) { y = value; };
    void setZ(float value) { z = value; };

    void translate(float dX, float dY, float dZ);
    void lookAt(Vector3D lookPos) { lookingAt = lookPos; };
    void display();

    Vector3D applyTransform(const Vector3D &lightPosition);
};

#endif /* CAMERA_H */