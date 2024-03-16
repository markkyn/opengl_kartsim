#include <GL/glu.h>
#include <stdint.h>

#include "./camera.h"
#include "../math/matrix.h"

Camera::Camera(float sX, float sY, float sZ)
{
    gluLookAt(sX, sY, sZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    this->x = sX;
    this->y = sY;
    this->z = sZ;

    lookingAt = Vector3D(0, 0, 0);
    upVector = Vector3D(0, 1, 0);
}

void Camera::translate(float dX, float dY, float dZ)
{
    /* Creating Translate Matrix */
    Matrix translation_matrix(4, 4);

    // setting identity
    for (int i = 0; i < 4; i++)
        translation_matrix.setValue(i, i, 1);

    translation_matrix.setValue(3, 0, dX);
    translation_matrix.setValue(3, 1, dY);
    translation_matrix.setValue(3, 2, dZ);

    /* Position Matrix */
    Matrix position_matrix(1, 4);
    position_matrix.setValue(0, 0, this->x);
    position_matrix.setValue(0, 1, this->y);
    position_matrix.setValue(0, 2, this->z);
    position_matrix.setValue(0, 3, 1);

    Matrix newPos_matrix = position_matrix * translation_matrix;

    this->x = newPos_matrix.getValue(0, 0);
    this->y = newPos_matrix.getValue(0, 1);
    this->z = newPos_matrix.getValue(0, 2);
}

void Camera::display()
{
    float lookX = this->lookingAt.getX();
    float lookY = this->lookingAt.getY();
    float lookZ = this->lookingAt.getZ();

    float upX = this->upVector.getX();
    float upY = this->upVector.getY();
    float upZ = this->upVector.getZ();

    gluLookAt(this->x, this->y, this->z, lookX, lookY, lookZ, upX, upY, upZ);
}