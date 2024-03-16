// gameobjects.cpp

#include "../math/vectors.h"
#include "../math/matrix.h"
#include "./gameobjects.h"

#include <fstream>
#include <sstream>
#include <iostream>
// #include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "../../include/objloader.hpp"

GameObject::GameObject(const char *objFileName)
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
    scaleValue = 0.01;

    bool res = loadOBJ(objFileName, vertices, uvs, normals);

    std::cout << "resultado = " << res << std::endl;
}

void GameObject::drawModel()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES); // Mudar para GL_TRIANGLES, GL_LINES, etc., conforme necessário.

    for (auto &vertex : vertices)
        glVertex3f(vertex.x  + this->x, vertex.y + this->y, vertex.z + this->z);
    glEnd();
    glutSwapBuffers();
}

void GameObject::translate(Vector3D to_pos)
{
    /* Creating Translate Matrix */
    Matrix translation_matrix(4, 4);

    float dX = this->getX() - to_pos.getX();
    float dY = this->getY() - to_pos.getY();
    float dZ = this->getZ() - to_pos.getZ();

    // setting identity
    for (int i = 0; i < 4; i++)
        translation_matrix.setValue(i, i, 1);

    translation_matrix.setValue(4, 0, dX);
    translation_matrix.setValue(4, 1, dY);
    translation_matrix.setValue(4, 2, dZ);

    Matrix position_matrix(1, 4);
    position_matrix.setValue(0, 0, this->x);
    position_matrix.setValue(0, 1, this->y);
    position_matrix.setValue(0, 2, this->z);
    position_matrix.setValue(0, 3, 1);

    Matrix resultMatrix = position_matrix * translation_matrix;

    this->x = resultMatrix.getValue(0, 0);
    this->y = resultMatrix.getValue(0, 1);
    this->z = resultMatrix.getValue(0, 2);
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