// gameobjects.cpp

#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./gameobjects.h"
#include "../math/matrix.h"
#include "../math/vectors.h"
#include "../../include/objloader.hpp"

/* PRIVATE */
void GameObject::drawModel()
{
    GLfloat mat_specular[] = {1.0, 1.0, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.3, 0.3, 1.0};
    GLfloat mat_shininess[] = {30.0};

    glPushAttrib(GL_LIGHTING_BIT);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glEnable(GL_LIGHTING);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_TRIANGLES); // Mudar para GL_TRIANGLES, GL_LINES, etc., conforme necessário.

    for (auto &vertex : vertices)
        glVertex3f(vertex.x, vertex.y, vertex.z);
    glEnd();
    glutSwapBuffers();
    glDisable(GL_LIGHTING);
}

/* PUBLIC */
GameObject::GameObject(const char *objFileName)
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
    scaleValue = 0.01;
    cameraPtr = nullptr;
    this->centerOfMass = glm::vec3(x, y, z);
    forward = Vector3D(1, 0, 0);

    bool res = loadOBJ(objFileName, vertices, uvs, normals);
}

void GameObject::display()
{

    /* Attached Camera */
    if (cameraPtr)
    {
        /* CameraPos */
        float cameraOffsetX = -4.0f;
        float cameraOffsetY = 4.0f;
        float cameraOffsetZ = 0.0f;

        cameraPtr->setX(this->getX() + cameraOffsetX);
        cameraPtr->setY(this->getY() + cameraOffsetY);
        cameraPtr->setZ(this->getZ() + cameraOffsetZ);

        float lookOffsetX = 0.0f;
        float lookOffsetY = 0.0f;
        float lookOffsetZ = 0.0f;

        /* LookAt */
        Vector3D lookAtCar(
            this->x + lookOffsetX,
            this->y + lookOffsetY,
            this->z + lookOffsetZ);
        cameraPtr->lookAt(lookAtCar);

        cameraPtr->display();
    }

    this->drawModel();
}

void GameObject::translate(Vector3D to_pos)
{
    /* Creating Translate Matrix */
    Matrix translation_matrix(4, 4);

    float dX = to_pos.getX();
    float dY = to_pos.getY();
    float dZ = to_pos.getZ();

    // setting identity
    for (int i = 0; i < 4; i++)
        translation_matrix.setValue(i, i, 1);

    translation_matrix.setValue(3, 0, dX);
    translation_matrix.setValue(3, 1, dY);
    translation_matrix.setValue(3, 2, dZ);

    Matrix position_matrix(1, 4);
    for (auto &vertex : vertices)
    {
        position_matrix.setValue(0, 0, vertex.x);
        position_matrix.setValue(0, 1, vertex.y);
        position_matrix.setValue(0, 2, vertex.z);
        position_matrix.setValue(0, 3, 1);
        Matrix resultMatrix = position_matrix * translation_matrix;

        vertex.x = resultMatrix.getValue(0, 0);
        vertex.y = resultMatrix.getValue(0, 1);
        vertex.z = resultMatrix.getValue(0, 2);
    }

    this->x = this->x + dX;
    this->y = this->y + dY;
    this->z = this->z + dZ;

    this->centerOfMass = glm::vec3(x, y, z);
}

void GameObject::scale(float x, float y, float z)
{
    Matrix scaleMatrix(4, 4);
    scaleMatrix.setValue(0, 0, x);
    scaleMatrix.setValue(1, 1, y);
    scaleMatrix.setValue(2, 2, z);
    scaleMatrix.setValue(3, 3, 1);

    Matrix position_matrix(1, 4);
    for (auto &vertex : vertices)
    {
        position_matrix.setValue(0, 0, vertex.x);
        position_matrix.setValue(0, 1, vertex.y);
        position_matrix.setValue(0, 2, vertex.z);
        position_matrix.setValue(0, 3, 1);

        Matrix resultMatrix = position_matrix * scaleMatrix;

        vertex.x = resultMatrix.getValue(0, 0);
        vertex.y = resultMatrix.getValue(0, 1);
        vertex.z = resultMatrix.getValue(0, 2);
    }
}

void GameObject::rotate(float x, float y, float z)
{
    /* x matrix */
    Matrix X_rotateMatrix(3, 3);
    X_rotateMatrix.setValue(0, 0, 1);
    X_rotateMatrix.setValue(1, 1, cos(x));
    X_rotateMatrix.setValue(1, 2, -sin(x));
    X_rotateMatrix.setValue(2, 1, sin(x));
    X_rotateMatrix.setValue(2, 2, cos(x));

    /* y matrix */
    Matrix Y_rotateMatrix(3, 3);
    Y_rotateMatrix.setValue(0, 0, cos(y));
    Y_rotateMatrix.setValue(0, 2, sin(y));
    Y_rotateMatrix.setValue(1, 1, 1);
    Y_rotateMatrix.setValue(2, 0, -sin(y));
    Y_rotateMatrix.setValue(2, 2, cos(y));

    /* z matrix */
    Matrix Z_rotateMatrix(3, 3);
    Z_rotateMatrix.setValue(0, 0, cos(z));
    Z_rotateMatrix.setValue(0, 1, -sin(z));
    Z_rotateMatrix.setValue(1, 0, sin(z));
    Z_rotateMatrix.setValue(1, 1, cos(z));

    Matrix general_rotateMatrix = X_rotateMatrix * Y_rotateMatrix * Z_rotateMatrix;

    Matrix position_matrix(1, 3);
    for (auto &vertex : vertices)
    {
        position_matrix.setValue(0, 0, vertex.x);
        position_matrix.setValue(0, 1, vertex.y);
        position_matrix.setValue(0, 2, vertex.z);

        Matrix resultMatrix = position_matrix * general_rotateMatrix;

        vertex.x = resultMatrix.getValue(0, 0);
        vertex.y = resultMatrix.getValue(0, 1);
        vertex.z = resultMatrix.getValue(0, 2);
    }
}

void GameObject::rotateQuat(double angle, Vector3D axis)
{
    double angleRadians = angle * M_PI / 180.0;

    glm::quat rotationQuat = glm::angleAxis((float)angleRadians, glm::vec3(axis.getX(), axis.getY(), axis.getZ()));

    for (glm::vec3 &vertex : vertices)
    {
        glm::vec3 translatedVertex = vertex - centerOfMass;

        glm::vec3 rotatedVertex = rotationQuat * translatedVertex;

        vertex = rotatedVertex + centerOfMass;
    }

    // Update attributes
    this->centerOfMass = glm::vec3(x, y, z);

    // updateForward
    glm::vec3 eulerAngles = glm::eulerAngles(rotationQuat);

    float angleX =  eulerAngles.x;
    float angleY = eulerAngles.y;
    float angleZ =  eulerAngles.z;

    forward.rotate(angleX, angleY, angleZ);
}
