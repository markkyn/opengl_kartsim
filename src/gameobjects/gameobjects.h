#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/glm.hpp>

#include "../math/vectors.h"
#include "./camera.h"
#include "./terrain.h"

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
    float rotateX, rotateY, rotateZ;

    glm::vec3 centerOfMass;

    Vector3D cameraOffset;
    Camera *cameraPtr = nullptr;
    Terrain *terrainPtr = nullptr;

    /* Properties */
    float scaleValue;

    void drawModel();
    void updateForwardVector();
    void readObj(const std::string &objFileName);

public:
    Vector3D forward;

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

    void setX(float value) { x = value; };
    void setY(float value) { y = value; };
    void setZ(float value) { z = value; };
};

#endif /* GAMEOBJECTS_H*/