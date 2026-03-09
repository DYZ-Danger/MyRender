#pragma once
#include <vector>
#include "Structure.h"

class Texture
{
public:
    int width, height;
    std::vector<unsigned int> data;
    
    Texture(int w, int h);
    
    Vector4f Sample(Vector2f uv);
    void LoadTexture(const std::string& path);
};
