#pragma once
#include <windows.h>
#include <string>

#include "Structure.h"
using namespace std;

struct Sample
{
    Vector4f color;
    float z;
};
 class Window
 {
     public:
         Window(int w, int h, const wstring& name);
         ~Window();
     
        void Clear(unsigned int color);
        void Present() const;
        void DrawPixel(int x, int y, float z, unsigned int color);
        static bool ProcessMessage();
        unsigned int* _framebuffer = nullptr;
        // float* _zbuffer = nullptr;
        // unsigned int* _superframebuffer = nullptr;
        // float* _superzbuffer = nullptr;
        void DrawFPS(float fps) const;
        void DrawMSAA(int x, int y, float z, const Vector4f& color, float a, float b, float g, 
            float d_adx, float d_ady, float d_bdx, float d_bdy);
        bool DepthTest(int x, int y, float oneDivZ);
     
        vector<Sample> msaaBuffer;
        int _width, _height;
 
     private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        HWND _hwnd;
        wstring _classname;
        HINSTANCE _hInstance;
        BITMAPINFO _bmi;
 };