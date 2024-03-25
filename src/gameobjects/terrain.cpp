#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <GL/freeglut.h>

#include "stb_image.h"

#include "terrain.h"

int terrain_texture_width, terrain_texture_height, terrain_texture_channels;
static GLuint textureTerrainID;

void Terrain::loadTerrainTexture(const char *textura)
{
    // load da textura
    unsigned char *terrain_image = stbi_load(textura, &terrain_texture_width, &terrain_texture_height, &terrain_texture_channels, 0);

    // declara um objeto de textura
    glGenTextures(1, &textureTerrainID);

    // cria e usa objetos de textura
    glBindTexture(GL_TEXTURE_2D, textureTerrainID);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // magnification e minification filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // define uma textura bidimensional
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, terrain_texture_width, terrain_texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, terrain_image);

    stbi_image_free(terrain_image);
    glDisable(GL_BLEND);
}

Terrain::Terrain(char *filename, const char *textura)
{
    this->width = 0;
    this->depth = 0;
    this->hightScale = 0.0055;

    this->offset_x = 0.0f;
    this->offset_y = 0.0f;
    this->offset_z = 0.0f;

    this->loadFile(filename);
    loadTerrainTexture(textura);
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
            topografy[j][i] = height * hightScale;
        }
    }

    file.close();
}

void Terrain::drawTerrain()
{
    // ativa textura
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, textureTerrainID);

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

            float uv_x = float(i) / (width - 2); // vai de 0.0 a 1.0 baseado no quao grande estah o I, pq o I vai ate (width-2) no maximo
            float uv_y = float(j) / (depth - 2);
            glTexCoord2f(uv_x, uv_y);

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
    glDisable(GL_TEXTURE_2D);

    glPopAttrib();
}

Vector3D Terrain::normalAt(float x, float y)
{
    float delta = 0.1f;
    float heightCenter = heightAt(x, y);
    float heightXPlus = heightAt(x + delta, y);
    float heightYPlus = heightAt(x, y + delta);

    glm::vec3 vecX(delta, heightXPlus - heightCenter, 0);
    glm::vec3 vecY(0, heightYPlus - heightCenter, delta);

    glm::vec3 normal = glm::cross(vecY, vecX);
    normal = glm::normalize(normal);

    if (normal.y < 0)
    {
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

Vector3D Terrain::diffZeroAt(float x, float y)
{
    int i0 = floor(x);
    int j0 = floor(y); // Ponto mais proximo da origem
    int i1 = ceil(y);
    int j1 = ceil(y); // Ponto mais longe da origem

    if (heightAt(i0, j0) == heightAt(i0, j1)) // ->
        return Vector3D(0.0f, 0.0f, 1.0f);
    if (heightAt(i0, j0) == heightAt(i1, j0)) // /
        return Vector3D(1.0f, 0.0f, 0.0);
    if (heightAt(i0, j0) == heightAt(i1, j1)) //
        return Vector3D(0.5f, 0.0f, 0.5f);

    return Vector3D(0.0, 1.0f, 0.0f);
}