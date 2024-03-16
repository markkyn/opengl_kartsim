#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include "../math/vectors.h"
#include <iostream>
#include <vector>

class GameObject
{
private:
    /* Global Position */
    float x, y, z;
    void readObj(const std::string &objFileName);
    std::vector<Vector3D> vertex;
    std::vector<Vector3D> vertexNormal;
    std::vector<Vector3D> vertexTexture;


public:
    GameObject(const std::string &objFileName);
    ~GameObject();

    void drawModel();

    void translate(Vector3D to_pos);
    void rotate();
    void scale(float x, float y, float z);

    float getX() { return x; };
    float getY() { return y; };
    float getZ() { return z; };
};

#endif /* GAMEOBJECTS_H*/