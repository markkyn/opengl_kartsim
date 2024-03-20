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
#include "./gameobjects/terrain.h"

#define MIN_8BIT 0;
#define MAX_8BIT 255;

#define DEBUG false

GLfloat luz_pontual[] = {0.3, 0.5, 0.5, 1.0};
Camera *camera;
GameObject *car;
Terrain *terrain;

/* Helpers */
Vector3D yAxis(0.0f, 1.0f, 0.0f);
Vector3D zAxis(0.0f, 0.0f, 1.0f);
Vector3D xAxis(1.0f, 0.0f, 0.0f);


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

void init(char **argv)
{
    glClearColor(0.2, 0.2, 0.2, 0.0);

    iluminar();

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    /* Camera */
    camera = new Camera(4, 2, 3);
    /* Terrain */
    terrain = new Terrain(argv[1]);

    /* GameObj = Car */
    car = new GameObject("../assets/car_formula.obj");

    car->attachCamera(camera);
    car->attachTerrain(terrain);
    car->scale(0.01, 0.01, 0.01);

    car->rotateQuat(-90, xAxis);

}

void specialKeys(int key, int x, int y)
{
    float angulo = 2 * M_PI / 180;
    switch (key)
    {
    case GLUT_KEY_UP:
    {
        car->translate(car->getForward() * 0.1);
        break;
    }
    case GLUT_KEY_LEFT:
    {
        car->rotateQuat(5, yAxis);
        break;
    }
    case GLUT_KEY_RIGHT:
    {
        car->rotateQuat(-5, yAxis);

        break;
    }
    case GLUT_KEY_DOWN:
        car->translate(car->getForward() * -0.1);
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
    desenhar_luz();

    camera->display();
    car->display();
    terrain->drawTerrain();

    std::cout << "Height At (" << car->getX() << "," << car->getZ() << ") = " << terrain->heightAt(car->getX(), car->getZ()) << std::endl;
    std::cout << "Normal At (" << car->getX() << "," << car->getZ() << ") = (" 
        << terrain->normalAt(car->getX(), car->getZ()).getX() << " , "
        << terrain->normalAt(car->getX(), car->getZ()).getY() << " , "
        << terrain->normalAt(car->getX(), car->getZ()).getZ() << " , "
        << ")"<< std::endl;

    desenhar_eixos();

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

    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Hello World");

    /* Creating Objects */
    /* Reading Terrain File */
    init(argv);

    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}
