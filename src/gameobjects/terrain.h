#ifndef TERRAIN_H
#define TERRAIN_H

#include <iostream>
#include <vector>
#include "../math/vectors.h"

class Terrain
{
private:
    int width, depth;
    float hightScale;
    std::vector<std::vector<float>> topografy;
    float offset_x, offset_y, offset_z;

    void loadFile(char *file_name);

public:
    Terrain(char *filename);
    ~Terrain();

    Vector3D normalAt(int x, int y);
    float heightAt(int x, int y) { return topografy[x][y]; };
    void drawTerrain();
};

#endif /* TERRAIN_H */