#ifndef CAMERA_H
#define CAEMRA_H


#include <GL/freeglut.h>

class Camera
{
private:
    GLdouble x, y, z; // position
    GLdouble i, j, k; // direction
    GLdouble zNear;
    GLdouble fov;
public:
    Camera();
    void lookAt();
    void rotate();
    void translate();
};

#endif /* CAMERA_H */
