#ifndef MATERIAL_H
#define MATERIAL

#include <GL/gl.h>
#include <GL/glut.h>

class Material
{
private:
    GLfloat specular[4]  = {1.0, 1.0, 0.0, 1.0};
    GLfloat diffuse[4]   = {1.0, 0.3, 0.3, 1.0};
    GLfloat shininess[1] = {30.0};

public:
    Material();
    ~Material();
};

Material::Material()
{
}

Material::~Material()
{
}

#endif /* MATERIAL_H*/