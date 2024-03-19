#include <iostream>
#include <stdio.h>
#include "terrain.h"

#include <GL/gl.h>
#include <GL/freeglut.h>

Terrain::Terrain()
{
    width = 0;
    depth = 0;
}

void Terrain::loadFile(char *file_name){
    FILE* file = fopen(file_name, "r");

    std::string ppm_type;

    fscanf(file, "%s", &ppm_type);
    if (ppm_type != "P2")
    {
        return;
    }

    int maxHeight = 0;
    fscanf(file, "%d %d", &width, &depth);

    topografy = (float *) malloc(sizeof(float) * width * depth);

    fscanf(file, "%d", &maxHeight );


    for(int px; px < width * depth; px++)
        fscanf(file, "%d", topografy[depth * 10 + width] * this->hightScale);

    fclose(file);
}

void Terrain::drawTerrain()
{
    float offset_x = 4;
    float offset_z = 4;
    float offset_y = 4;

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

    int escala = 1;
    for (int i = 0; i < width - 1; i++)
        for (int j = 0; j < depth - 1; j++)
        {
            glVertex3f(
                i - offset_x,
                altitude[i][j] - offset_y,
                j - offset_z);

            glVertex3f(
                i + 1 - offset_x,
                altitude[i + 1][j] - offset_z,
                j - offset_y);

            glVertex3f(
                i + 1 - offset_x,
                altitude[i + 1][j + 1] - offset_y,
                j + 1 - offset_z);

            glVertex3f(
                i - offset_x,
                altitude[i][j + 1] - offset_y,
                j + 1 - offset_z);
        }
    glEnd();

    glDisable(GL_LIGHTING);
    glPopAttrib();
}

Vector3D normalAt(int x, int y)
{
    this->heightAt(x, y);
    
    Vector3D gradient(
        x - x0,
        y - y0,
        z - z0
    );


    Vector3D position(

    );

    Vector3D normal = position * gradient;
}