#ifndef TERRENO_H
#define TERRENO_H

#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class Terrain
{
private:
    float width;
    float depth;
    vector<vector<float>> height;

public:
    Terrain(float width, float depth);

    Terrain(const string &nomeArquivo);

    float getWidth();

    float getDepht();
    
    float getHeight(float x, float y);

};

#endif /* TERRENO_H */
