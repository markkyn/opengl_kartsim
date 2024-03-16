// CG - T01 - Marcos Gabriel, Gustavo;
// main.cpp
// g++ -o main ./src/main.cpp ./src/math/matrix.cpp ./src/gameobjects/camera.cpp -lGL -lGLU -lglut
// ./main ./assets/file.ppm

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include <vector>

#include "./gameobjects/camera.h"
#include "./gameobjects/gameobjects.h"

#define MIN_8BIT 0;
#define MAX_8BIT 255;

#define DEBUG false

GLfloat luz_pontual[] = {0.3, 0.5, 0.5, 1.0};
Camera *camera;
GameObject *car;


void iluminar()
{
    GLfloat light0_position[] = {0.0, 1.0, 0.0, 0.0};
    GLfloat light0_diffuse[] = {0.1, 0.1, 0.1, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);

    GLfloat light1_diffuse[] = {0.6, 0.6, 0.6, 1.0};
    GLfloat light1_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light1_ambient[] = {0.1, 0.1, 0.1, 1.0};

    glLightfv(GL_LIGHT1, GL_POSITION, luz_pontual);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void desenhar_luz()
{

    glPushAttrib(GL_LIGHTING_BIT);

    GLfloat mat_diffuse[] = {1.0, 1.0, 0.0, 1.0};
    GLfloat mat_emission[] = {1.0, 1.0, 0.0, 1.0};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);

    glPushMatrix();
    glTranslatef(luz_pontual[0], luz_pontual[1], luz_pontual[2]);

    glEnable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 0.0);
    glutSolidSphere(0.05, 50, 50);
    glDisable(GL_LIGHTING);

    glPopAttrib();
    glPopMatrix();
}

void desenhar_eixos()
{
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();
}

void desenhar_objeto()
{
    GLfloat mat_specular[] = {1.0, 1.0, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.3, 0.3, 1.0};
    GLfloat mat_shininess[] = {30.0};

    glPushAttrib(GL_LIGHTING_BIT);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glEnable(GL_LIGHTING);

    glBegin(GL_QUADS);
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(5.0f, 0.0f, -5.0f);
    glVertex3f(5.0f, 0.0f, 5.0f);
    glVertex3f(-5.0f, 0.0f, 5.0f);
    glEnd();
    glDisable(GL_LIGHTING);

    glPopAttrib();
}
// Terrain

std::vector<std::vector<int>> altitude;
int32_t terrain_width, terrain_depth;

void load_file(const std::string &file_name)
{
    std::ifstream file(file_name);

    if (!file.is_open())
    {
        std::cerr << "Não foi possivel ler o arquivo" << std::endl;
        return;
    }

    std::string ppm_type;
    file >> ppm_type;
    std::cout << "[Terrain File]  " << ppm_type << std::endl;
    if (ppm_type != "P2")
    {
        std::cerr << "Formato invalido" << std::endl;
        return;
    }

    int32_t max_value;
    file >> terrain_width >> terrain_depth;
    file >> max_value;
    std::cout << "[Terrain File] "
              << "w = " << terrain_width << "  d = " << terrain_depth << std::endl
              << "[Terrain File] m = " << max_value << std::endl;

    altitude.resize(terrain_width);
    for (int i = 0; i < terrain_width; ++i)
        altitude[i].resize(terrain_depth);

    for (int i = 0; i < terrain_width - 1; i++)
        for (int j = 0; j < terrain_depth - 1; j++)
            file >> altitude[i][j];

    file.close();
}

void desenhar_terreno()
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
    for (int i = 0; i < terrain_width - 1; i++)
        for (int j = 0; j < terrain_depth - 1; j++)
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

void init(void)
{
    glClearColor(0.2, 0.2, 0.2, 0.0);

    iluminar();

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    camera = new Camera(4, 2, 3);
    car = new GameObject("../assets/car_formula.obj");
    car->scale(0.01, 0.01, 0.01);
}

void specialKeys(int key, int x, int y)
{
    float angulo = 2 * M_PI / 180;
    switch (key)
    {
    case GLUT_KEY_UP:
        camera->translate(0.0, 0.5, 0.0);
        break;
    case GLUT_KEY_LEFT:
        camera->translate(-0.5, 0.0, 0.0);
        break;
    case GLUT_KEY_RIGHT:
        camera->translate(0.5, 0.0, 0.0);
        break;
    case GLUT_KEY_DOWN:
        camera->translate(0.0,-0.5, 0.0);
        break;
    }
    glutPostRedisplay();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLightfv(GL_LIGHT1, GL_POSITION, luz_pontual);

    camera->display();
    car->drawModel();

    desenhar_luz();
    desenhar_eixos();
    //desenhar_terreno();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h); // Define o viewport para ocupar toda a janela

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (GLfloat)w / (GLfloat)h, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glfwInit();
    
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello World");

    /* Creating Objects */
    /* Reading Terrain File */
    load_file(argv[1]);
    init();

    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}
