#include "Camera.h"
#include <cmath>

const float PI = acos(-1.0f);

Camera::Camera()
    :eye(2, 0.4f, 0),
    up(0, -1, 0)
{
    UpdateForward();
}

Matrix4f Camera::GetViewMatrix()
{
    return Matrix4f::LookAt(eye, eye + forward, up);
}

void Camera::ProcessKeyboard(unsigned char key, float deltaTime)
{
    float speed = 2.5f * deltaTime;
    if (key == 'W')
    {
        eye = eye + forward * speed;
    }
    else if (key == 'S')
    {
        eye = eye - forward * speed;
    }
    Vector3f right = forward.Product(up);
    if (key == 'A')
    {
        eye = eye - right * speed;
    }
    else if (key == 'D')
    {
        eye = eye + right * speed;
    }
    if (key == ' ')
    {
        eye = eye + up * speed;
    }
    else if (key == 0x11)
    {
        eye = eye - up * speed;
    }
}

void Camera::UpdateForward()
{
    Vector3f result;
    result.x = cos(yaw * PI / 180) * cos(pitch * PI / 180);
    result.y = sin(pitch * PI / 180);
    result.z = sin(yaw * PI / 180) * cos(pitch * PI / 180);
    this->forward = result.Normalize();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw -= xoffset;
    pitch -= yoffset;
    
    pitch = std::min(pitch, 89.0f);
    pitch = std::max(pitch, -89.0f);
    
    UpdateForward();
}

void Camera::Reset()
{
    this->eye = {2.0f, 0.4f, 0.0f};
    this->up = {0.0f, -1.0f, 0.0f};
    this->yaw = 0.0f;
    this->pitch = 0.0f;
    UpdateForward();
}
