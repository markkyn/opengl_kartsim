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
    Vector3D forward;
    Vector3D up;

    GLuint textureID;

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
    // void readObj(const std::string &objFileName);
    void alignWithTerrainNormal(Vector3D normalAtPoint);

    /* Physics */
    float speed;
    float maxSpeed;
    // float acelleration; // nao usado
    Vector3D direction; // wheel direction

public:
    GameObject(const char *objFileName, const char *textura);

    void attachCamera(Camera *camera) { cameraPtr = camera; };
    void attachTerrain(Terrain *terrain) { terrainPtr = terrain; };

    void translate(Vector3D to_pos);
    void rotate(float x, float y, float z);
    void rotateQuat(double angle, Vector3D axis);
    void scale(float x, float y, float z);

    void display();

    void loadTexture(const char *textura);
    
    float getX() { return x; };
    float getY() { return y; };
    float getZ() { return z; };

    void setX(float value) { x = value; };
    void setY(float value) { y = value; };
    void setZ(float value) { z = value; };
    Vector3D getForward() { return forward; };

    void setSpeed(float value) { speed = value; };
    void setMaxSpeed(float value) { maxSpeed = value; };
    // void setAcelleration(float value) { acelleration = value; };
    
    float getSpeed() { return speed; };
    float getMaxSpeed() { return maxSpeed; };
    // float getAcelleration() { return acelleration; };

    // void acellerate();


};

#endif /* GAMEOBJECTS_H*/