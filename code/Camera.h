#pragma once
#include "Structure.h"

class Camera
{
public:
    Vector3f eye;
    Vector3f up;
    Vector3f forward;
    float yaw = 0.0f;
    float pitch = 0.0f;
    
    Camera();
    
    Matrix4f GetViewMatrix();
    
    void ProcessKeyboard(unsigned char key, float deltaTime);
    void UpdateForward();
    void ProcessMouseMovement(float xoffset, float yoffset);
    void Reset();
};
