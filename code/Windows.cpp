#include "Windows.h"
#include <cmath>
using namespace std;

Window::Window(int w, int h, const wstring& name)
    :_classname(L"Software Render"), 
    _hInstance(GetModuleHandle(nullptr)),
    _width(w), 
    _height(h)
{
    WNDCLASS wc = {};
    
    wc.lpfnWndProc = Window::WindowProc;
    wc.hInstance = _hInstance;
    wc.lpszClassName = _classname.c_str();
    
    RegisterClass(&wc);
    
    _framebuffer = new unsigned int[static_cast<size_t>(w) * h];
    // _zbuffer = new float[static_cast<size_t>(w) * h];
    // _superframebuffer = new unsigned int[static_cast<size_t>(w) * h * 4];
    // _superzbuffer = new float[static_cast<size_t>(w) * h * 4];
    msaaBuffer.resize(static_cast<size_t>(w) * h * 4);
    _bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    _bmi.bmiHeader.biBitCount = 32;
    _bmi.bmiHeader.biCompression = BI_RGB;
    _bmi.bmiHeader.biWidth = w;
    _bmi.bmiHeader.biHeight = -h;
    _bmi.bmiHeader.biPlanes = 1;
    
    _hwnd = CreateWindowEx(
        0,
        _classname.c_str(),
        name.c_str(),
        WS_OVERLAPPEDWINDOW,
        
        CW_USEDEFAULT, CW_USEDEFAULT, w, h,
        nullptr, nullptr, _hInstance, nullptr
    );
    
    if (_hwnd == nullptr)
    {
        return;
    }
    ShowWindow(_hwnd, SW_SHOW);
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
Window::~Window()
{
    DestroyWindow(_hwnd);
    delete[] _framebuffer;
    // delete[] _zbuffer;
    UnregisterClass(_classname.c_str(), _hInstance);
}

void Window::Clear(unsigned int color)
{
    //fill_n(_framebuffer, _width * _height, color);
    
    float r = ((color >> 16) & 0xFF) / 255.0f;
    float g = ((color >> 8) & 0xFF) / 255.0f;
    float b = (color & 0xFF) / 255.0f;
    Vector4f bgColor(r, g, b, 1.0f);
    Sample defaultSample;
    defaultSample.color = bgColor;
    defaultSample.z = -1e6;
    fill(msaaBuffer.begin(), msaaBuffer.end(), defaultSample);
    // fill_n(_zbuffer, _width * _height, -1e10f);
    // fill_n(_superframebuffer, _width * _height * 4, color);
    // fill_n(_superzbuffer, _width * _height * 4, -1e10f);
}

void Window::Present() const
{
    HDC hdc = GetDC(_hwnd);
    StretchDIBits(hdc,
        0,
        0,
        _width,
        _height,
        0,
        0,
        _width,
        _height,
        _framebuffer,
        &_bmi,
        DIB_RGB_COLORS,
        SRCCOPY);
    ReleaseDC(_hwnd, hdc);
}

// void Window::DrawPixel(int x, int y, float z, unsigned int color)
// {
//     if (x >= _width * 2 || y >= _height * 2 || x < 0 || y < 0)
//     {
//         return;
//     }
//     int index = y * _width * 2 + x;
//     if (z > _superzbuffer[index])
//     {
//         _superzbuffer[index] = z;
//         _superframebuffer[index] = color;
//     }
// }

void Window::DrawMSAA(int x, int y, float z, const Vector4f& color, float a, float b, float g,
    float d_adx, float d_ady, float d_bdx, float d_bdy)
{
    if (x < 0 || x >= _width || y < 0 || y >= _height) return;
    
    float offsets[4][2] = {
        {0.375f, 0.125f}, {-0.125f, 0.375f}, 
        {0.125f, -0.375f}, {-0.375f, -0.125f}
    };
    
    int pixelIdx = (static_cast<size_t>(y) * _width + static_cast<size_t>(x)) * 4;
    for (int i = 0; i < 4; i ++)
    {
        float sa = a + offsets[i][0] * d_adx + offsets[i][1] * d_ady;
        float sb = b + offsets[i][0] * d_bdx + offsets[i][1] * d_bdy;
        float sg = 1.0f - sa - sb;
        
        if (sa >= 0 && sb >= 0 && sg >= 0)
        {
            if (z >= msaaBuffer[pixelIdx + i].z)
            {
                msaaBuffer[pixelIdx + i].color = color;
                msaaBuffer[pixelIdx + i].z = z;
            }
        }
    }
}

bool Window::ProcessMessage()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT) return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

void Window::DrawFPS(float fps) const
{
    HDC hdc = GetDC(_hwnd);
    
    // 设置文字样式
    SetTextColor(hdc, RGB(255, 255, 0)); // 黄色文字
    SetBkMode(hdc, TRANSPARENT);         // 背景透明

    // 格式化字符串
    wstring fpsStr = L"FPS: " + to_wstring(static_cast<int>(fps));

    // 绘制到左上角 (10, 10) 坐标处
    TextOut(hdc, 10, 10, fpsStr.c_str(), static_cast<int>(fpsStr.length()));

    ReleaseDC(_hwnd, hdc);
}

bool Window::DepthTest(int x, int y, float oneDivZ)
{
    if (x < 0 || x >= _width || y < 0 || y >= _height) return false;

    int pixelIdx = (y * _width + x) * 4;
    
    // MSAA 优化逻辑：只要当前深度值比 4 个子采样点中任意一个“更近”，就说明没被完全遮挡
    // 注意：根据你的 Pipeline.cpp，使用的是 oneDivZ，值越大代表离相机越近
    for (int i = 0; i < 4; i++)
    {
        if (oneDivZ > msaaBuffer[pixelIdx + i].z) 
        {
            return true; 
        }
    }
    
    return false;
}
