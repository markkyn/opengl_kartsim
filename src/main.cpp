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


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define MIN_8BIT 0;
#define MAX_8BIT 255;

#define DEBUG false

GLfloat luz_pontual[] = {0.3, 0.5, 0.5, 1.0};
GLfloat light2_position[] = {5, 1.0, 5.0, 1, 1.0};

Camera *camera;
GameObject *car;
Terrain *terrain;
GameObject* pneus[5];

/* Helpers */
Vector3D yAxis(0.0f, 1.0f, 0.0f);
Vector3D zAxis(0.0f, 0.0f, 1.0f);
Vector3D xAxis(1.0f, 0.0f, 0.0f);

// controle das special keys
int is_up_pressed = 0;
int is_down_pressed = 0;
int is_left_pressed = 0;
int is_right_pressed = 0;

// carSpeed pra frente e pra tras do carro
float MAX_SPEED = 0.4;
float carSpeed = 0.0;


void iluminar()
{
    glEnable(GL_LIGHTING);

    GLfloat light1_diffuse[] = {0.6, 0.6, 0.6, 1.0};
    GLfloat light1_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light1_ambient[] = {0.1, 0.1, 0.1, 1.0};

    glLightfv(GL_LIGHT1, GL_POSITION, luz_pontual);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);

    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light1_ambient);

    //glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
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

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    /* Camera */
    camera = new Camera(4, 2, 3);    

    /* Terrain */
    // terrain = new Terrain(argv[1], "../assets/textura_teste_uv.jpg"); // pra testar certinho, eh bom colocar uma malha (imagem ppm) de tamanho igual ou maior q essa textura de teste
    terrain = new Terrain(argv[1], "../assets/textura_terreno_png.png");

    /* GameObj = Car */
    car = new GameObject("../assets/carro.obj", "../assets/textura_carro.jpg");

    for(int i=0; i<5; i++){
        pneus[i] = new GameObject("../assets/pneus.obj", "../assets/textura_pneus_png.png");
    }

    car->attachCamera(camera);
    car->attachTerrain(terrain);
    car->setMaxSpeed(MAX_SPEED);

    car->translate(Vector3D(27.0, 0.0, 12.5)); // posicao inicial do mapa

    pneus[0]->translate(Vector3D(37.35, 0.0, 17.25));
}

void specialKeyPressed(int key, int x, int y){
    if(key == GLUT_KEY_UP) is_up_pressed = 1;
    if(key == GLUT_KEY_DOWN) is_down_pressed = 1;
    if(key == GLUT_KEY_LEFT) is_left_pressed = 1;
    if(key == GLUT_KEY_RIGHT) is_right_pressed = 1;
}

void specialKeyReleased(int key, int x, int y){
    if(key == GLUT_KEY_UP) is_up_pressed = 0;
    if(key == GLUT_KEY_DOWN) is_down_pressed = 0;
    if(key == GLUT_KEY_LEFT) is_left_pressed = 0;
    if(key == GLUT_KEY_RIGHT) is_right_pressed = 0;
}

void handle_car_movement(){
    if(is_up_pressed) carSpeed += 0.005;
    if(is_down_pressed) carSpeed -= 0.005;

    // clamp dos valores da carSpeed
    if(carSpeed > MAX_SPEED) carSpeed = MAX_SPEED;
    if(carSpeed < -MAX_SPEED) carSpeed = -MAX_SPEED;
    
    // controle da rotacao levando em consideracao a re (dando re roda ao contrario) (nao compara exatamente com 0 pq a desaceleracao eh uma interpolacao)
    if(carSpeed < -0.01){
        if(is_left_pressed) car->rotateQuat(-5, yAxis);
        if(is_right_pressed) car->rotateQuat(5, yAxis);
    }
    else{
        if(is_left_pressed) car->rotateQuat(5, yAxis);
        if(is_right_pressed) car->rotateQuat(-5, yAxis);
    }
    
    car->translate(car->getForward() * carSpeed);

    // desaceleracao (fiz com *0.1 para ir diminuindo gradualmente e n ter chance de diminuir dms alem do 0 e ficar um valor negativo)
    if(!is_up_pressed && !is_down_pressed){
        if(carSpeed > 0.0) carSpeed -= carSpeed*0.05;
        if(carSpeed < 0.0) carSpeed += -carSpeed*0.05;
    }

    car->setSpeed(carSpeed);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //desenhar_luz();

    camera->display();

    
    car->display();
    terrain->drawTerrain();
    desenhar_eixos();

    for(int i=0; i<5; i++){
        pneus[i]->display();
    }


    iluminar();

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


void gameloop(){
    handle_car_movement();
    glutPostRedisplay();
}

void timerFunc(int value){
    gameloop();
    glutTimerFunc(1000 / 60, timerFunc, 0); // Configura o próximo chamada da função timerFunc(), roda a 60fps
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glfwInit();

    // glutInitWindowSize(1920, 1080);
    glutInitWindowSize(1366, 762);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Hello World");

    /* Creating Objects */
    /* Reading Terrain File */
    init(argv);

    glutDisplayFunc(display);
    glutSpecialFunc(specialKeyPressed);
    glutSpecialUpFunc(specialKeyReleased);
    glutReshapeFunc(reshape);
    
    glutTimerFunc(1000 / 60, timerFunc, 0); // 60 FPS
    glutMainLoop();

    return 0;
}
