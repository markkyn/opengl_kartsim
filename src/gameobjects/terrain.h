#ifndef TERRAIN_H  
#define TERRAIN_H

#include <iostream>
#include <vector>
#include "vectors.h"


class Terrain
{
private:
    int width, depth;
    float hightScale;
    float* topografy;

public:
    Terrain();
    ~Terrain();

    Vector3D normalAt(int x, int y);
    float heightAt(int x, int y);
    void  drawTerrain();
    void  loadFile(char *file_name);
};

#endif /* TERRAIN_H */