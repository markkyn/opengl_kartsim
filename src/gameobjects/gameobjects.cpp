// gameobjects.cpp

#include "../math/vectors.h"
#include "../math/matrix.h"
#include "./gameobjects.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glut.h>
#include "../../include/objloader.hpp"

GameObject::GameObject(const std::string &objFileName)
{
    this->readObj(objFileName);
}

void GameObject::readObj(const std::string &objFileName)
{
    std::ifstream file(objFileName);
    std::string line;
    while (getline(file, line))
    {
        std::istringstream lineStream(line);
        std::string lineType;
        lineStream >> lineType;
        if (lineType == "v")
        {
            float x, y, z;
            lineStream >> x >> y >> z;
            vertex.push_back(Vector3D(x, y, z));
        }
        else if (lineType == "vn")
        {
            float x, y, z;
            lineStream >> x >> y >> z;
            vertexNormal.push_back(Vector3D(x, y, z));
        }
    }
}

void GameObject::drawModel()
{
    glBegin(GL_TRIANGLE_STRIP);
    for (auto vert : vertex)
        glVertex3f(vert.getX() + this->x, vert.getY() + this->y, vert.getZ() + this->y);
    glEnd();
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
}