#include "Windows.h"
#include "Pipeline.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    Window win(800, 600, L"Render");
    Pipeline my_pipeline(win);
    SimpleShader shader;
    my_pipeline.UsingShader(&shader);
    Camera my_camera;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    int framecount = 0;
    float timer = 0.0f;
    float currentFPS = 0.0f;
    
    Mesh teapot;
    teapot.LoadObj("cup.obj");
    std::string info = "Total Faces: " + std::to_string(teapot.indices.size() / 3) + "\n";
    OutputDebugStringA(info.c_str());
    shader.mytexture->LoadTexture("cup.jpg");
    while (win.ProcessMessage())
    {
        float currentFrame = GetTickCount() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        framecount ++;
        timer += deltaTime;
        if (timer > 1.0f)
        {
            currentFPS = static_cast<float>(framecount) / timer;
            framecount = 0;
            timer = 0.0f;
        }
        
        if (GetAsyncKeyState('W'))
        {
            my_camera.ProcessKeyboard('W', deltaTime);
        }
        if (GetAsyncKeyState('S'))
        {
            my_camera.ProcessKeyboard('S', deltaTime);
        }
        if (GetAsyncKeyState('A'))
        {
            my_camera.ProcessKeyboard('A', deltaTime);
        }
        if (GetAsyncKeyState('D'))
        {
            my_camera.ProcessKeyboard('D', deltaTime);
        }
        if (GetAsyncKeyState(' '))
        {
            my_camera.ProcessKeyboard(' ', deltaTime);
        }
        if (GetAsyncKeyState(VK_CONTROL))
        {
            my_camera.ProcessKeyboard(VK_CONTROL, deltaTime);
        }
        if (GetAsyncKeyState('R'))
        {
            my_camera.Reset();
        }
        
        POINT mousePos;
        GetCursorPos(&mousePos);
        static POINT lastMousePos = mousePos;
        float xoffset = static_cast<float>(mousePos.x - lastMousePos.x);
        float yoffset = static_cast<float>(lastMousePos.y - mousePos.y);
        lastMousePos = mousePos;
        
        if (GetAsyncKeyState(VK_LBUTTON))
        {
            my_camera.ProcessMouseMovement(xoffset, yoffset);
        }
        
        win.Clear(0x0000FF);
        my_pipeline.Render(my_camera, teapot);
        win.Present();
        
        win.DrawFPS(currentFPS);
    }
    return 0;
}
