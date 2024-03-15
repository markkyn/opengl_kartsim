#ifndef CAMERA_H
#define CAMERA_H

#include "./gameobjects.h"

class Camera
{
private:
    float x, y, z; // Global Position
    bool activated;

public:
    Camera(float sX, float sY, float sZ);

    /* Getters */
    float getX() { return x; };
    float getY() { return y; };
    float getZ() { return z; };

    void translate(float dX, float dY, float dZ);
    void display();
};

#endif /* CAMERA_H */