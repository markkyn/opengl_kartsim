#ifndef VECTORS_H
#define VECTORS_H

class Vector3D
{
private:
    float x, y, z;
public:
    Vector3D(float x, float y, float z);
    ~Vector3D();

    rotate(Quaternion quat);
    translate(float x,float y, float z);
};

class Quaternion
{
private:
    float i, j, k, l;

public:
    Quaternion(float i, float j, float k, float l);
};
