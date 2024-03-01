#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include "../math/vectors.h"

class GameObject
{
private:
    /* Global Position */
    float x, y, z;

public:
    GameObject();
    ~GameObject();

    void translate(Vector3D to_pos);
    void rotate();
    void scale(Vector3D to_scale);
};


#endif /* GAMEOBJECTS_H*/