#pragma once
#include "Structure.h"
#include "Texture.h"

class Shader
{
public:
    virtual VerToFrag VertexShader(const Vertex& vex) = 0;
    virtual Vector4f FragmentShader(const VerToFrag& vex) = 0;
    
    Matrix4f ModelMatrix;
    Matrix4f ViewMatrix;
    Matrix4f ProjectionMatrix;
    
    Texture* mytexture = new Texture(256, 256);
};

class SimpleShader: public Shader
{
    public:
        virtual VerToFrag VertexShader(const Vertex& vex) override;
        virtual Vector4f FragmentShader(const VerToFrag& vex) override;
};