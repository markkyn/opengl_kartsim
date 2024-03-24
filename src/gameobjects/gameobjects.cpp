// gameobjects.cpp

#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./gameobjects.h"
#include "../math/matrix.h"
#include "../math/vectors.h"
#include "../../include/objloader.hpp"

#include "stb_image.h"

int image_width, image_height, channels;


/* PRIVATE */
void GameObject::drawModel()
{
    // ativa textura
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLfloat mat_specular[] = {1.0, 1.0, 1, 1.0};
    GLfloat mat_diffuse[] = {1.0, 1, 1, 1.0};
    GLfloat mat_shininess[] = {50.0};

    glPushAttrib(GL_LIGHTING_BIT);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glEnable(GL_LIGHTING);
       
    glBegin(GL_TRIANGLES); // Mudar para GL_TRIANGLES, GL_LINES, etc., conforme necessário.

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        glNormal3f(normals[i].x, normals[i].y+ this->y, normals[i].z);
        glTexCoord2f(uvs[i].x, uvs[i].y);
        glVertex3f(vertices[i].x, vertices[i].y + this->y, vertices[i].z);
    }
    glEnd();


    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glPopAttrib();
}

void GameObject::alignWithTerrainNormal(Vector3D normalAtPoint)
{
    Vector3D globalUp(0, 1, 0);

    Vector3D projectedNormal(normalAtPoint.getX(), 0, normalAtPoint.getZ());
    Vector3D projectedUp(globalUp.getX(), 0, globalUp.getZ());

    projectedNormal = projectedNormal.normalize();
    projectedUp = projectedUp.normalize();

    float dotProd = projectedUp.dot(projectedNormal);
    float angle = acos(dotProd) * (180.0 / M_PI); // Convertendo para graus

    if (projectedNormal.getX() > projectedUp.getX())
    {
        angle = -angle;
    }

    if (std::abs(angle) > 0.01)
    {
        Vector3D pitchAxis(0, 1, 0);
        this->rotateQuat(angle, pitchAxis);
    }
}

void GameObject::loadTexture(const char *textura)
{
    // load da textura
    unsigned char *image = stbi_load(textura, &image_width, &image_height, &channels, 0);

    // declara um objeto de textura
    glGenTextures(1, &textureID);

    // cria e usa objetos de textura
    glBindTexture(GL_TEXTURE_2D, textureID);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // magnification e minification filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // define uma textura bidimensional
    if(hasTransparency){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    }
    else{
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }

    stbi_image_free(image);
    glDisable(GL_BLEND);
}

/* PUBLIC */
GameObject::GameObject(const char *objFileName, const char *textura, bool transparency) // o modelo3D texturizado so fica com sombreamento se tiver com transparencia=true, e sua textura nao for 100% opaca, padrao: 90%
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
    scaleValue = 1.0; // 0.01; (alterei para 1.0 pois consertei o tamanho exagerado do modelo3D, e retirei o scale(0.01) do main)

    cameraPtr = nullptr;
    terrainPtr = nullptr;

    this->centerOfMass = glm::vec3(x, y, z);
    forward = Vector3D(1, 0, 0);
    up = Vector3D(1, 0, 0);

    /* Physics */
    direction = forward;

    bool res = loadOBJ(objFileName, vertices, uvs, normals);

    hasTransparency = transparency;
    loadTexture(textura);
}

void GameObject::display()
{

    if (terrainPtr)
    {
        this->setY(terrainPtr->heightAt(this->x, this->z));
        terrainPtr->drawTerrain();

        this->alignWithTerrainNormal(
            terrainPtr->normalAt((int)x, (int)z));
    }

    /* Attached Camera */
    if (cameraPtr)
    {
        /* CameraPos */
        float XZoffsetMultiplier = 4.0;
        float Yoffset = 3.0;

        float speedEffect = speed/maxSpeed; // efeito de olhar mais pra frente conforme vai mais rapido, vai de 0.0 a 1.0, eh 0 se tiver parado e 1.0 se tiver na velocidade maxima

        Vector3D carPos = Vector3D(centerOfMass.x, centerOfMass.y, centerOfMass.z);
        Vector3D cameraPosOffset = Vector3D(
            forward.getX()*XZoffsetMultiplier, 
            -Yoffset + speedEffect*0.75,                             // eh negativo pq esse vetor vai ser subtrarido da posicao do carro
            forward.getZ()*XZoffsetMultiplier
        );

        Vector3D cameraPos = carPos - cameraPosOffset;

        std::cout << centerOfMass.x << " " << centerOfMass.y << " " << centerOfMass.z << "\n";

        cameraPtr->setX(cameraPos.getX());
        cameraPtr->setY(cameraPos.getY());
        cameraPtr->setZ(cameraPos.getZ());

        float lookOffsetX = 0.0f;
        float lookOffsetY = 1.0f;
        float lookOffsetZ = 0.0f;

        /* LookAt */
        Vector3D lookAtCar(
            this->x + lookOffsetX,
            this->y + lookOffsetY,
            this->z + lookOffsetZ);
        cameraPtr->lookAt(lookAtCar);
    }

    this->drawModel();
}

void GameObject::translate(Vector3D to_pos)
{
    /* Creating Translate Matrix */
    Matrix translation_matrix(4, 4);

    float dX = to_pos.getX();
    float dY = to_pos.getY();
    float dZ = to_pos.getZ();

    // setting identity
    for (int i = 0; i < 4; i++)
        translation_matrix.setValue(i, i, 1);

    translation_matrix.setValue(3, 0, dX);
    translation_matrix.setValue(3, 1, dY);
    translation_matrix.setValue(3, 2, dZ);

    Matrix position_matrix(1, 4);
    for (auto &vertex : vertices)
    {
        position_matrix.setValue(0, 0, vertex.x);
        position_matrix.setValue(0, 1, vertex.y);
        position_matrix.setValue(0, 2, vertex.z);
        position_matrix.setValue(0, 3, 1);
        Matrix resultMatrix = position_matrix * translation_matrix;

        vertex.x = resultMatrix.getValue(0, 0);
        vertex.y = resultMatrix.getValue(0, 1);
        vertex.z = resultMatrix.getValue(0, 2);
    }

    this->x = this->x + dX;
    this->y = this->y + dY;
    this->z = this->z + dZ;

    this->centerOfMass = glm::vec3(x, y, z);
}

void GameObject::scale(float x, float y, float z)
{
    Matrix scaleMatrix(4, 4);
    scaleMatrix.setValue(0, 0, x);
    scaleMatrix.setValue(1, 1, y);
    scaleMatrix.setValue(2, 2, z);
    scaleMatrix.setValue(3, 3, 1);

    Matrix position_matrix(1, 4);
    for (auto &vertex : vertices)
    {
        position_matrix.setValue(0, 0, vertex.x);
        position_matrix.setValue(0, 1, vertex.y);
        position_matrix.setValue(0, 2, vertex.z);
        position_matrix.setValue(0, 3, 1);

        Matrix resultMatrix = position_matrix * scaleMatrix;

        vertex.x = resultMatrix.getValue(0, 0);
        vertex.y = resultMatrix.getValue(0, 1);
        vertex.z = resultMatrix.getValue(0, 2);
    }
}

void GameObject::rotate(float x, float y, float z)
{
    /* x matrix */
    Matrix X_rotateMatrix(3, 3);
    X_rotateMatrix.setValue(0, 0, 1);
    X_rotateMatrix.setValue(1, 1, cos(x));
    X_rotateMatrix.setValue(1, 2, -sin(x));
    X_rotateMatrix.setValue(2, 1, sin(x));
    X_rotateMatrix.setValue(2, 2, cos(x));

    /* y matrix */
    Matrix Y_rotateMatrix(3, 3);
    Y_rotateMatrix.setValue(0, 0, cos(y));
    Y_rotateMatrix.setValue(0, 2, sin(y));
    Y_rotateMatrix.setValue(1, 1, 1);
    Y_rotateMatrix.setValue(2, 0, -sin(y));
    Y_rotateMatrix.setValue(2, 2, cos(y));

    /* z matrix */
    Matrix Z_rotateMatrix(3, 3);
    Z_rotateMatrix.setValue(0, 0, cos(z));
    Z_rotateMatrix.setValue(0, 1, -sin(z));
    Z_rotateMatrix.setValue(1, 0, sin(z));
    Z_rotateMatrix.setValue(1, 1, cos(z));

    Matrix general_rotateMatrix = X_rotateMatrix * Y_rotateMatrix * Z_rotateMatrix;

    Matrix position_matrix(1, 3);
    for (auto &vertex : vertices)
    {
        position_matrix.setValue(0, 0, vertex.x);
        position_matrix.setValue(0, 1, vertex.y);
        position_matrix.setValue(0, 2, vertex.z);

        Matrix resultMatrix = position_matrix * general_rotateMatrix;

        vertex.x = resultMatrix.getValue(0, 0);
        vertex.y = resultMatrix.getValue(0, 1);
        vertex.z = resultMatrix.getValue(0, 2);
    }
}

void GameObject::rotateQuat(double angle, Vector3D axis)
{
    double angleRadians = angle * M_PI / 180.0;

    glm::quat rotationQuat = glm::angleAxis((float)angleRadians, glm::vec3(axis.getX(), axis.getY(), axis.getZ()));

    for (glm::vec3 &vertex : vertices)
    {
        glm::vec3 translatedVertex = vertex - centerOfMass;

        glm::vec3 rotatedVertex = rotationQuat * translatedVertex;

        vertex = rotatedVertex + centerOfMass;
    }

    // Update attributes
    this->centerOfMass = glm::vec3(x, y, z);

    // updateForward
    glm::vec3 eulerAngles = glm::eulerAngles(rotationQuat);

    float angleX = eulerAngles.x;
    float angleY = eulerAngles.y;
    float angleZ = eulerAngles.z;

    forward.rotate(angleX, angleY, angleZ);
}
