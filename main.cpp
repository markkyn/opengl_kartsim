// main.cpp
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

void display(void){

}

void init(void){
    glClearColor(0.0,0.0,0.0,1);
}

int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutCreateWindow("Kart Sim");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
}