#ifndef CAMERA_H
#define CAMERA_H

#include "vectors.h"

class Camera
{
private:
public:
    Camera();
    ~Camera();
    lookAt(Vector3D direction);
};

Camera::Camera()
{
}

Camera::~Camera()
{
}
