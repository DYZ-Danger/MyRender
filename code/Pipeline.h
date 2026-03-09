#pragma once
#include "Windows.h"
#include "Structure.h"
#include "Shader.h"
#include "Camera.h"

class Pipeline
{
public:
    Pipeline(Window& win);
    ~Pipeline() = default;
    Window& device;
    //void DrawLine(const VerToFrag& v1, const VerToFrag& v2);
    Shader* CurrentShader;
    void UsingShader(Shader* shader);
    void DrawCube(const Mesh& cube, const Matrix4f& model, const Matrix4f& view, const Matrix4f& projection);
    void Render(Camera& my_camera, Mesh& MyCube);
    void ScanLine(VerToFrag v1, VerToFrag v2, VerToFrag v3);
    // void FillTopTriangle(VerToFrag v1, VerToFrag v2, VerToFrag v3);
    // void FillBottomTriangle(VerToFrag v1, VerToFrag v2, VerToFrag v3);
    void FillTriangle(const VerToFrag& v1, const VerToFrag& v2, const VerToFrag& v3);
    unsigned int ColorToInt(const Vector4f _color);
    int ClipWithPlane(const VerToFrag* inVertices, int inCount, int plane_idx, VerToFrag* outVertices);
    VerToFrag TransformToScreen(VerToFrag& v);
    bool BackfaceCulling(const VerToFrag& vex0, const VerToFrag& vex1, const VerToFrag& vex2);
    //void ResolveSSAA();
    void ResolveMSAA();
    void ComputeBarycentric(float x, float y, const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, float& a, float& b, float& c, float invArea);
};
