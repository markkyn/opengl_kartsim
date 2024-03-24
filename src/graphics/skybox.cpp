#include "skybox.h"
#include "stb_image.h"
#include <GL/freeglut.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GLES3/gl3.h>

Skybox::Skybox(std::vector<std::string> faces)
{

    float scale = 40.0f;
    float skyboxVertices[] = {
        //   Coordinates
        -scale, -scale, scale,  //        7--------6
        scale, -scale, scale,   //       /|       /|
        scale, -scale, -scale,  //      4--------5 |
        -scale, -scale, -scale, //      | |      | |
        -scale, scale, scale,   //      | 3------|-2
        scale, scale, scale,    //      |/       |/
        scale, scale, -scale,   //      0--------1
        -scale, scale, -scale};

    unsigned int skyboxIndices[] = {
        // Right
        1, 2, 6,
        6, 5, 1,
        // Left
        0, 4, 7,
        7, 3, 0,
        // Top
        4, 5, 6,
        6, 7, 4,
        // Bottom
        0, 3, 2,
        2, 1, 0,
        // Back
        0, 1, 5,
        5, 4, 0,
        // Front
        3, 7, 6,
        6, 2, 3};

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    cubemapTexture = loadCubemap(faces);
}

unsigned int Skybox::loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                width,
                height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data);
            stbi_image_free(data);
            std::cout << "texture: " << faces[i] << std::endl;

        }
        else
        {
            std::cerr << "Failed to load texture: " << faces[i] << std::endl;
            stbi_image_free(data);
            return 0;
        }
    }

    return textureID;
}

void Skybox::display()
{
    glEnable(GL_TEXTURE_CUBE_MAP);
    glDepthFunc(GL_LEQUAL);

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
    glDisable(GL_TEXTURE_CUBE_MAP);

}