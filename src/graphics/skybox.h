#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/gl.h>
#include <GL/freeglut.h>

#include <iostream>
#include <vector>

class Skybox
{
private:
    int width, height;
    int nrChannels;
    
    float skyboxVertices[24];
    unsigned int skyboxIndices[36];


    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    unsigned int cubemapTexture;

    std::vector<std::string> faces;
    unsigned int loadCubemap(std::vector<std::string> faces);

public:
    Skybox(std::vector<std::string> faces);
    void display();

};

#endif /* SKYBOX_H */ 