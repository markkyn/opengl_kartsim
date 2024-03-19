#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/glm.hpp>

#include "../math/vectors.h"
#include "./camera.h"

class GameObject
{
protected:
    /* Global Position */
    float x, y, z;

    /* Points */
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    uint vertexBuffer;

    glm::vec3 centerOfMass;

    Camera *cameraPtr = nullptr;
    Vector3D cameraOffset;

    Vector3D forward;

    /* Properties */
    float scaleValue;

    void readObj(const std::string &objFileName);
    void drawModel();

public:
    GameObject(const char *objFileName);
    void attachCamera(Camera *camera) { cameraPtr = camera; };

    void translate(Vector3D to_pos);
    void rotate(float x, float y, float z);
    void rotateQuat(double angle, Vector3D axis);
    void scale(float x, float y, float z);

    void display();

    float getX() { return x; };
    float getY() { return y; };
    float getZ() { return z; };
};

#endif /* GAMEOBJECTS_H*/