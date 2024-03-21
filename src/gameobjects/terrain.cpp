#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <GL/freeglut.h>

#include "terrain.h"

Terrain::Terrain(char *filename)
{
    this->width = 0;
    this->depth = 0;
    this->hightScale = 0.15;

    this->offset_x = 0.0f;
    this->offset_y = 0.0f;
    this->offset_z = 0.0f;

    this->loadFile(filename);
}

void Terrain::loadFile(char *file_name)
{
    std::ifstream file(file_name);

    if (!file.is_open())
    {
        std::cerr << "Não foi possivel ler o arquivo" << std::endl;
        return;
    }

    std::string ppm_type;
    file >> ppm_type;

    if (ppm_type != "P2")
    {
        std::cerr << "Formato Inválido, insira o pathing para o arquivo ppm do terreno." << std::endl;
        return;
    }

    int maxHeight = 0;
    file >> width >> depth;
    file >> maxHeight;

    float height = 0.0;

    /* Alocation */
    topografy.resize(width);
    for (int w = 0; w < width; w++)
        topografy[w].resize(depth);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < depth; j++)
        {
            file >> height;
            topografy[i][j] = height * hightScale;
        }
    }

    file.close();
}

void Terrain::drawTerrain()
{
    GLfloat mat_specular[] = {1.0, 1.0, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.3, 0.3, 1.0};
    GLfloat mat_shininess[] = {30.0};

    glColor3f(0.5f, 0.7f, 0.3f);

    glPushAttrib(GL_LIGHTING_BIT);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glBegin(GL_QUADS);

    for (int i = 0; i < width - 1; i++)
        for (int j = 0; j < depth - 1; j++)
        {
            Vector3D normal = normalAt(i, j); 
            glNormal3f(normal.getX(), normal.getY(), normal.getZ());

            glVertex3f(
                i - offset_x,
                topografy[i][j],
                j - offset_z);

            glVertex3f(
                i + 1 - offset_x,
                topografy[i + 1][j],
                j - offset_z);

            glVertex3f(
                i + 1 - offset_x,
                topografy[i + 1][j + 1],
                j + 1 - offset_z);

            glVertex3f(
                i - offset_x,
                topografy[i][j + 1],
                j + 1 - offset_z);
        }
    glEnd();

    glPopAttrib();
}

Vector3D Terrain::normalAt(float x, float y) {
    float delta = 0.1f;
    float heightCenter = heightAt(x, y);
    float heightXPlus = heightAt(x + delta, y);
    float heightYPlus = heightAt(x, y + delta);

    glm::vec3 vecX(delta, heightXPlus - heightCenter, 0);
    glm::vec3 vecY(0, heightYPlus - heightCenter, delta);

    glm::vec3 normal = glm::cross(vecX, vecY);
    normal = glm::normalize(normal);

    if (normal.y < 0) {
        normal = -normal;
    }

    return Vector3D(normal.x, normal.y, normal.z);
}

float Terrain::heightAt(float x, float y)
{
    // Type casting float -> int
    int x0 = static_cast<int>(x);
    int y0 = static_cast<int>(y);

    if (x0 < 0 || x0 >= width - 1 || y0 < 0 || y0 >= depth - 1)
    {
        return 0;
    }

    // position diff
    float dx = x - x0;
    float dy = y - y0;

    // Interpolation
    float top = topografy[x0][y0] * (1 - dx) + topografy[x0 + 1][y0] * dx;
    float bottom = topografy[x0][y0 + 1] * (1 - dx) + topografy[x0 + 1][y0 + 1] * dx;

    // Getting Height via interpolation
    float height = top * (1 - dy) + bottom * dy;

    return height;
}
