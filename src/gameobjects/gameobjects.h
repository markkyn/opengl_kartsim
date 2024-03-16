#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include "../math/vectors.h"
#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/glm.hpp>

class GameObject
{
private:
    /* Global Position */
    float x, y, z;
    
    /* Points */
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    uint vertexBuffer;

    /* Properties */
    float scaleValue;

    void readObj(const std::string &objFileName);

public:
    GameObject(const char *objFileName);
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