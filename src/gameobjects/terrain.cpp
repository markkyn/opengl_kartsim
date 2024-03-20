#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include <GL/gl.h>
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

    glEnable(GL_LIGHTING);

    glBegin(GL_QUADS);

    for (int i = 0; i < width - 1; i++)
        for (int j = 0; j < depth - 1; j++)
        {
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

    glDisable(GL_LIGHTING);
    glPopAttrib();
}

Vector3D Terrain::normalAt(int x, int y)
{
    if (x <= 0 || x >= width - 1 || y <= 0 || y >= depth - 1)
    {
        return Vector3D(0, 0, 1);
    }

    Vector3D v1(x - 1, y, topografy[y][x - 1] * hightScale);
    Vector3D v2(x + 1, y, topografy[y][x + 1] * hightScale);
    Vector3D v3(x, y - 1, topografy[y - 1][x] * hightScale);
    Vector3D v4(x, y + 1, topografy[y + 1][x] * hightScale);

    Vector3D diag1 = v2 - v1;
    Vector3D diag2 = v4 - v3;

    Vector3D normal = diag1.cross(diag2);
    normal = normal.normalize(); // Normalizar o vetor resultante

    return normal;
}