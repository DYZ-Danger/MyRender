#include "Pipeline.h"
#include <cmath>

// void Pipeline::DrawLine(const VerToFrag& v1, const VerToFrag& v2)
// {
//     int x1 = static_cast<int>(v1.screen_position.x);
//     int y1 = static_cast<int>(v1.screen_position.y);
//     int x2 = static_cast<int>(v2.screen_position.x);
//     int y2 = static_cast<int>(v2.screen_position.y);
//     
//     if (x1 < -1000 || x1 > 2000) {
//         OutputDebugStringA("Error: Invalid screen coordinates!\n");
//     }
//     
//     int dx = abs(x2 - x1);
//     int dy = abs(y2 - y1);
//     int sx = (x1 < x2) ? 1 : -1;
//     int sy = (y1 < y2) ? 1 : -1;
//     
//     int err = dx - dy;
//     int step = max(dx, dy);
//     VerToFrag stepV;
//     if (step > 0)
//     {
//         stepV = (v2 - v1) * (1.0f / static_cast<float>(step));
//     }
//     VerToFrag currentV = v1;
//     
//     while (true)
//     {
//         Vector4f currentColor = CurrentShader->FragmentShader(currentV);
//         unsigned int a = static_cast<unsigned int>(max(0.0f, min(1.0f, currentColor.w)) * 255.0f);
//         unsigned int r = static_cast<unsigned int>(max(0.0f, min(1.0f, currentColor.x)) * 255.0f);
//         unsigned int g = static_cast<unsigned int>(max(0.0f, min(1.0f, currentColor.y)) * 255.0f);
//         unsigned int b = static_cast<unsigned int>(max(0.0f, min(1.0f, currentColor.z)) * 255.0f);
//         unsigned int finalColor = (a << 24) | (r << 16) | (g << 8) | b;
//         device.DrawPixel(x1, y1, finalColor);
//         
//         if (x1 == x2 && y1 == y2)
//         {
//             break;
//         }
//         int e2 = err * 2;
//         if (e2 > -dy)
//         {
//             err -= dy;
//             x1 += sx;
//         }
//         if (e2 < dx)
//         {
//             err += dx;
//             y1 += sy;
//         }
//         
//         currentV += stepV;
//     }
// }

// void Pipeline::DrawLine(const VerToFrag& v1, const VerToFrag& v2)
// {
//     // 1. 强制确定左右顺序，防止 length 为负导致不画线
//     const VerToFrag& left = (v1.screen_position.x < v2.screen_position.x) ? v1 : v2;
//     const VerToFrag& right = (v1.screen_position.x < v2.screen_position.x) ? v2 : v1;
//
//     int xStart = static_cast<int>(floor(left.screen_position.x + 0.5f)); // 向上取整确保覆盖
//     int xEnd = static_cast<int>(floor(right.screen_position.x + 0.5f));
//     int y = static_cast<int>(left.screen_position.y);
//     
//     if (y < 0 || y >= 1200) return;
//     
//     if (xStart > xEnd)
//     {
//         swap(xStart, xEnd);
//     }
//     
//     xStart = max(0, min(1600, xStart));
//     xEnd = max(0, min(1600, xEnd));
//
//     int length = xEnd - xStart;
//     
//     // 2. 处理极窄的情况，确保至少画一个像素
//     if (length <= 0) {
//         unsigned int color = ColorToInt(CurrentShader->FragmentShader(left));
//         device.DrawPixel(xStart, y, static_cast<float>(left.oneDivZ), color);
//         return;
//     }
//
//     // 3. 像素填充循环
//     for (int x = xStart; x <= xEnd; x++)
//     {
//         if (x < 0 || x >= 1600 || y < 0 || y >= 1200) continue;
//         float weight = static_cast<float>(x - xStart) / length;
//         // 线性插值所有顶点属性
//         VerToFrag currentV = left * (1.0f - weight);
//         currentV += right * weight;
//         
//         unsigned int finalColor = ColorToInt(CurrentShader->FragmentShader(currentV));
//         
//         device.DrawPixel(x, y, static_cast<float>(currentV.oneDivZ), finalColor);
//     }
// }


void Pipeline::UsingShader(Shader* shader)
{
    this->CurrentShader = shader;
}

void Pipeline::DrawCube(const Mesh& cube, const Matrix4f& model, const Matrix4f& view, const Matrix4f& projection)
{
    CurrentShader->ModelMatrix = model;
    CurrentShader->ViewMatrix = view;
    CurrentShader->ProjectionMatrix = projection;
    
    for (size_t i = 0; i < cube.indices.size(); i += 3)
    {
        Vertex v0 = cube.vertices[cube.indices[i]];
        Vertex v1 = cube.vertices[cube.indices[i + 1]];
        Vertex v2 = cube.vertices[cube.indices[i + 2]];
        
        VerToFrag vex0 = CurrentShader->VertexShader(v0);
        VerToFrag vex1 = CurrentShader->VertexShader(v1);
        VerToFrag vex2 = CurrentShader->VertexShader(v2);
        
        VerToFrag buffer1[16];
        VerToFrag buffer2[16];
        
        buffer1[0] = vex0;
        buffer1[1] = vex1;
        buffer1[2] = vex2;
        
        int currentCount = 3;
        VerToFrag* pIn = buffer1;
        VerToFrag* pOut = buffer2;

        // 2. 连续裁剪 6 个面
        bool fullyClipped = false;
        for (int j = 0; j < 6; j++) {
            currentCount = ClipWithPlane(pIn, currentCount, j, pOut);
            if (currentCount == 0) {
                fullyClipped = true;
                break;
            }
            // 交换输入输出指针
            std::swap(pIn, pOut);
        }

        if (fullyClipped) continue;

        // 3. 渲染裁剪后的多边形 (此时顶点都在 pIn 指向的数组里)
        for (int j = 1; j < currentCount - 1; j++) {
            VerToFrag out1 = TransformToScreen(pIn[0]);
            VerToFrag out2 = TransformToScreen(pIn[j]);
            VerToFrag out3 = TransformToScreen(pIn[j + 1]);
            
            if (!BackfaceCulling(out1, out2, out3)) {
                this->FillTriangle(out1, out2, out3);
            }
        }
        // VerToFrag out1 = TransformToScreen(vex0);
        // VerToFrag out2 = TransformToScreen(vex1);
        // VerToFrag out3 = TransformToScreen(vex2);
        // this->ScanLine(out1, out2, out3);
    }
}

void Pipeline::Render(Camera& my_camera, Mesh& MyCube)
{
    Matrix4f model = Matrix4f::Identity();
    Matrix4f view = my_camera.GetViewMatrix();
    Matrix4f proj = Matrix4f::Perspective(60.f, 1600.0f / 1200.0f, 0.1f, 100.0f);
    
    // Mesh triangle;
    // // 顶点0: 屏幕中心附近
    // triangle.vertices.push_back(Vertex(Vector4f(0.0f, 0.5f, 0.0f, 1.0f), Vector4f(1, 0, 0, 1))); // 红
    // // 顶点1: 偏左
    // triangle.vertices.push_back(Vertex(Vector4f(-1.5f, -0.5f, 0.0f, 1.0f), Vector4f(0, 1, 0, 1))); // 绿
    // // 顶点2: 偏右
    // triangle.vertices.push_back(Vertex(Vector4f(1.5f, -0.5f, 0.0f, 1.0f), Vector4f(0, 0, 1, 1))); // 蓝
    //
    // triangle.indices = { 0, 2, 1 };
    DrawCube(MyCube, model, view, proj);
    ResolveMSAA();
}

Pipeline::Pipeline(Window& win)
    : device(win)
{
    
}

void Pipeline::ScanLine(VerToFrag v1, VerToFrag v2, VerToFrag v3)
{
    
    float area = abs((v2.screen_position.x - v1.screen_position.x) * (v3.screen_position.y - v1.screen_position.y) - 
                     (v3.screen_position.x - v1.screen_position.x) * (v2.screen_position.y - v1.screen_position.y));
    if (abs(area) < 1e-6f) return;
    float invArea = 1.0f / area;
    
    if (v1.screen_position.y > v2.screen_position.y)
    {
        swap(v1, v2);
    }
    if (v1.screen_position.y > v3.screen_position.y)
    {
        swap(v1, v3);
    }
    if (v2.screen_position.y > v3.screen_position.y)
    {
        swap(v2, v3);
    }
    // if (abs(v1.screen_position.y - v2.screen_position.y) < 1e-6f) {
    //     FillBottomTriangle(v1, v2, v3);
    // } else if (abs(v2.screen_position.y - v3.screen_position.y) < 1e-6f) {
    //     FillTopTriangle(v1, v2, v3);
    // } else {
    //     // 3. 分割三角形
    //     float weight = (v2.screen_position.y - v1.screen_position.y) / (v3.screen_position.y - v1.screen_position.y);
    //     VerToFrag vex = v1 + (v3 - v1) * weight;
    //     vex.screen_position.y = v2.screen_position.y; // 强制对齐
    //
    //     // 上半部分：尖端是 v1，平底边是 v2 和 vex
    //     FillTopTriangle(v1, v2, vex); 
    //
    //     // 下半部分：平顶边是 v2 和 vex，尖端是 v3
    //     FillBottomTriangle(v2, vex, v3);
    // }
    // // FillBottomTriangle(v1, v2, v3);
    
    int yMin = static_cast<int>(floor(v1.screen_position.y + 0.5f));
    int yMax = static_cast<int>(floor(v3.screen_position.y + 0.5f));
    
    for (int y = yMin; y < yMax; y++)
    {
        float py = static_cast<float>(y) + 0.5f;
        float xl, xr;
        
        if (py < v2.screen_position.y) {
            // 上半部分：v1-v2 边 和 v1-v3 边
            xl = v1.screen_position.x + (v2.screen_position.x - v1.screen_position.x) * (py - v1.screen_position.y) / (v2.screen_position.y - v1.screen_position.y);
            xr = v1.screen_position.x + (v3.screen_position.x - v1.screen_position.x) * (py - v1.screen_position.y) / (v3.screen_position.y - v1.screen_position.y);
        } else {
            // 下半部分：v2-v3 边 和 v1-v3 边
            xl = v2.screen_position.x + (v3.screen_position.x - v2.screen_position.x) * (py - v2.screen_position.y) / (v3.screen_position.y - v2.screen_position.y);
            xr = v1.screen_position.x + (v3.screen_position.x - v1.screen_position.x) * (py - v1.screen_position.y) / (v3.screen_position.y - v1.screen_position.y);
        }

        if (xl > xr) std::swap(xl, xr);

        int xStart = static_cast<int>(floor(xl - 0.5f));
        int xEnd   = static_cast<int>(floor(xr + 0.5f));
        
        xStart = (std::max)(0, xStart);
        xEnd   = (std::min)(device._width - 1, xEnd);
        
        for (int x = xStart; x < xEnd; x ++)
        {
            float px = static_cast<float>(x) + 0.5f;
            
            float alpha = ((v2.screen_position.y - v3.screen_position.y) * (px - v3.screen_position.x) + (v3.screen_position.x - v2.screen_position.x) * (py - v3.screen_position.y)) * invArea;
            float beta  = ((v3.screen_position.y - v1.screen_position.y) * (px - v3.screen_position.x) + (v1.screen_position.x - v3.screen_position.x) * (py - v3.screen_position.y)) * invArea;
            float gamma = 1.0f - alpha - beta;
            
            VerToFrag currentV;
            currentV.color = v1.color * alpha + v2.color * beta + v3.color * gamma;
            currentV.normal = v1.normal * alpha + v2.normal * beta + v3.normal * gamma;
            currentV.oneDivZ = v1.oneDivZ * alpha + v2.oneDivZ * beta + v3.oneDivZ * gamma;
            currentV.uvOverZ = v1.uvOverZ * alpha + v2.uvOverZ * beta + v3.uvOverZ * gamma;
            
            // device.DrawMSAA(x, y, currentV.oneDivZ, CurrentShader->FragmentShader(currentV), 
            //                 v1.screen_position, v2.screen_position, v3.screen_position);
        }
    }
}

// void Pipeline::FillBottomTriangle(VerToFrag left_or_right1, VerToFrag left_or_right2, VerToFrag bottom)
// {
//     // bottom 是尖端（y最大）
//     VerToFrag t1 = left_or_right1;
//     VerToFrag t2 = left_or_right2;
//     if (t1.screen_position.x > t2.screen_position.x) swap(t1, t2);
//
//     int yStart = static_cast<int>(floor(t1.screen_position.y + 0.5f));
//     int yEnd   = static_cast<int>(floor(bottom.screen_position.y + 0.5f));
//
//     for (int y = yStart; y < yEnd; y++) {
//         float dy = bottom.screen_position.y - t1.screen_position.y;
//         float weight = (abs(dy) < 1e-6f) ? 0 : (float(y) + 0.5f - t1.screen_position.y) / dy;
//
//         VerToFrag left = t1 + (bottom - t1) * weight;
//         VerToFrag right = t2 + (bottom - t2) * weight;
//         this->DrawLine(left, right);
//     }
// }
//
// void Pipeline::FillTopTriangle(VerToFrag top, VerToFrag left_or_right1, VerToFrag left_or_right2)
// {
//     // top 是尖端（y最小）
//     VerToFrag b1 = left_or_right1;
//     VerToFrag b2 = left_or_right2;
//     if (b1.screen_position.x > b2.screen_position.x) swap(b1, b2);
//
//     int yStart = static_cast<int>(floor(top.screen_position.y + 0.5f));
//     int yEnd   = static_cast<int>(floor(b1.screen_position.y + 0.5f));
//
//     for (int y = yStart; y < yEnd; y++) {
//         float dy = b1.screen_position.y - top.screen_position.y;
//         float weight = (abs(dy) < 1e-6f) ? 0 : (float(y) + 0.5f - top.screen_position.y) / dy;
//         
//         VerToFrag left = top + (b1 - top) * weight;
//         VerToFrag right = top + (b2 - top) * weight;
//         this->DrawLine(left, right);
//     }
// }

void Pipeline::ComputeBarycentric(float x, float y, const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, float& a, float& b, float& c, float invArea)
{
    a = ((p2.y - p3.y) * (x - p3.x) + (p3.x - p2.x) * (y - p3.y)) * invArea;
    b = ((p3.y - p1.y) * (x - p3.x) + (p1.x - p3.x) * (y - p3.y)) * invArea;
    c = 1.0f - a - b;
}

void Pipeline::FillTriangle(const VerToFrag& v1, const VerToFrag& v2, const VerToFrag& v3)
{
    
    float minX = (std::min)({v1.screen_position.x, v2.screen_position.x, v3.screen_position.x});
    float maxX = (std::max)({v1.screen_position.x, v2.screen_position.x, v3.screen_position.x});
    float minY = (std::min)({v1.screen_position.y, v2.screen_position.y, v3.screen_position.y});
    float maxY = (std::max)({v1.screen_position.y, v2.screen_position.y, v3.screen_position.y});
    
    if (isnan(minX) || isnan(maxX) || isnan(minY) || isnan(maxY) || 
    isinf(minX) || isinf(maxX) || isinf(minY) || isinf(maxY)) {
        return;
    }
    
    // 确保范围能覆盖到屏幕之外一点点，靠 DrawMSAA 内部的 if (x<0) 挡住
    int xStart = static_cast<int>(floor(minX - 0.5f)); 
    int xEnd   = static_cast<int>(ceil(maxX + 0.5f));
    int yStart = static_cast<int>(floor(minY - 0.5f));
    int yEnd   = static_cast<int>(ceil(maxY + 0.5f));

    // 在循环开始前做限制，但给边缘留出 1 像素余量
    xStart = (std::max)(0, xStart);
    xEnd   = (std::min)(device._width - 1, xEnd);
    yStart = (std::max)(0, yStart);
    yEnd   = (std::min)(device._height - 1, yEnd);
    
    float area = (v2.screen_position.y - v3.screen_position.y) * (v1.screen_position.x - v3.screen_position.x) + 
                 (v3.screen_position.x - v2.screen_position.x) * (v1.screen_position.y - v3.screen_position.y);
    if (std::abs(area) < 1e-6f) return;
    float invArea = 1.0f / area;
    
    float d_alpha_dx = (v2.screen_position.y - v3.screen_position.y) * invArea;
    float d_alpha_dy = (v3.screen_position.x - v2.screen_position.x) * invArea;
    float d_beta_dx  = (v3.screen_position.y - v1.screen_position.y) * invArea;
    float d_beta_dy  = (v1.screen_position.x - v3.screen_position.x) * invArea;
    
    for (int y = yStart; y <= yEnd; y++)
    {
        for (int x = xStart; x <= xEnd; x++)
        {
            float alpha, beta, gamma;
            ComputeBarycentric(x + 0.5f, y + 0.5f, v1.screen_position, v2.screen_position, v3.screen_position, alpha, beta, gamma, invArea);
            //if (alpha < -0.0001f || beta < -0.0001f || gamma < -0.0001f) continue;
            float currentOneDivZ = v1.oneDivZ * alpha + v2.oneDivZ * beta + v3.oneDivZ * gamma;
            if (!device.DepthTest(x, y, currentOneDivZ)) continue;
            VerToFrag currentV;
            currentV.color = v1.color * alpha + v2.color * beta + v3.color * gamma;
            currentV.normal = v1.normal * alpha + v2.normal * beta + v3.normal * gamma;
            currentV.oneDivZ = currentOneDivZ;
            currentV.uvOverZ = v1.uvOverZ * alpha + v2.uvOverZ * beta + v3.uvOverZ * gamma;
                
            device.DrawMSAA(x, y, currentV.oneDivZ, CurrentShader->FragmentShader(currentV), alpha, beta, gamma,
                d_alpha_dx, d_alpha_dy, d_beta_dx, d_beta_dy);
        }
    }
}

unsigned int Pipeline::ColorToInt(const Vector4f _color)
{
    unsigned int a = static_cast<unsigned int>(max(0.0f, min(1.0f, _color.w)) * 255.0f);
    unsigned int r = static_cast<unsigned int>(max(0.0f, min(1.0f, _color.x)) * 255.0f);
    unsigned int g = static_cast<unsigned int>(max(0.0f, min(1.0f, _color.y)) * 255.0f);
    unsigned int b = static_cast<unsigned int>(max(0.0f, min(1.0f, _color.z)) * 255.0f);
    unsigned int finalColor = (a << 24) | (r << 16) | (g << 8) | b;
    return finalColor;
}

int Pipeline::ClipWithPlane(const VerToFrag* inVertices, int inCount, int plane_idx, VerToFrag* outVertices)
{
    int outCount = 0;
    if (inCount == 0) return 0;

    for (int i = 0; i < inCount; i++)
    {
        const VerToFrag& v1 = inVertices[i];
        const VerToFrag& v2 = inVertices[(i + 1) % inCount];
        
        auto getD = [&](const VerToFrag& v) -> float {
            switch (plane_idx) {
            case 0: return v.projection_position.w + v.projection_position.x;
            case 1: return v.projection_position.w - v.projection_position.x;
            case 2: return v.projection_position.w + v.projection_position.y;
            case 3: return v.projection_position.w - v.projection_position.y;
            case 4: return v.projection_position.w - 0.1f;                     
            case 5: return v.projection_position.w - v.projection_position.z;
            default: return 0.0f;
            }
        };

        float d1 = getD(v1);
        float d2 = getD(v2);
        bool v1Inside = (d1 >= 0);
        bool v2Inside = (d2 >= 0);

        // Sutherland-Hodgman 算法逻辑
        if (v1Inside && v2Inside)
        {
            outVertices[outCount++] = v2;
        }
        else if (v1Inside && !v2Inside)
        {
            float t = d1 / (d1 - d2);
            outVertices[outCount++] = v1 + (v2 - v1) * t;
        }
        else if (!v1Inside && v2Inside)
        {
            float t = d1 / (d1 - d2);
            outVertices[outCount++] = v1 + (v2 - v1) * t;
            outVertices[outCount++] = v2;
        }
    }
    return outCount;
}

VerToFrag Pipeline::TransformToScreen(VerToFrag& v)
{
    VerToFrag result = v;
    float w = v.projection_position.w;
    if (abs(w) < 0.0001f) w = 0.0001f; // 强制保护
    float invW = 1.0f / w;
    
    // 计算结果后，强制限制在屏幕范围附近，防止溢出 int 范围
    result.screen_position.x = (v.projection_position.x * invW + 1.0f) * 0.5f * device._width;
    result.screen_position.y = (1.0f - v.projection_position.y * invW) * 0.5f * device._height;
    
    // 限制在合理范围，比如 [-100, width + 100]
    result.screen_position.x = max(-100.0f, min(result.screen_position.x, (float)device._width + 100.0f));
    result.screen_position.y = max(-100.0f, min(result.screen_position.y, (float)device._height + 100.0f));
    
    return result;
}

bool Pipeline::BackfaceCulling(const VerToFrag& vex0, const VerToFrag& vex1, const VerToFrag& vex2)
{
    float x1 = vex0.screen_position.x;
    float x2 = vex1.screen_position.x;
    float x3 = vex2.screen_position.x;
    float y1 = vex0.screen_position.y;
    float y2 = vex1.screen_position.y;
    float y3 = vex2.screen_position.y;
        
    float cross_product = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
    if (cross_product < 0)
    {
        return true;
    }
    return false;
}

// void Pipeline::ResolveSSAA()
// {
//     const int targetW = 800;
//     const int targetH = 600;
//     
//     // 必须 Y 在外层循环，匹配行优先存储
//     for (int j = 0; j < targetH; j++) 
//     {
//         for (int i = 0; i < targetW; i++)
//         {
//             int sx = i * 2;
//             int sy = j * 2;
//             
//             确保全部使用 1600 作为跨度 (Stride)
//             unsigned int p1 = device._superframebuffer[sy * 1600 + sx];
//             unsigned int p2 = device._superframebuffer[sy * 1600 + (sx + 1)];
//             unsigned int p3 = device._superframebuffer[(sy + 1) * 1600 + sx];
//             unsigned int p4 = device._superframebuffer[(sy + 1) * 1600 + (sx + 1)];
//             
//             unsigned int r = (((p1 >> 16) & 0xFF) + ((p2 >> 16) & 0xFF) + ((p3 >> 16) & 0xFF) + ((p4 >> 16) & 0xFF)) >> 2;
//             unsigned int g = (((p1 >> 8) & 0xFF) + ((p2 >> 8) & 0xFF) + ((p3 >> 8) & 0xFF) + ((p4 >> 8) & 0xFF)) >> 2;
//             unsigned int b = ((p1 & 0xFF) + (p2 & 0xFF) + (p3 & 0xFF) + (p4 & 0xFF)) >> 2;
//             
//             写入 800 宽度的目标缓冲区
//             device._framebuffer[j * 800 + i] = (0xFF000000) | (r << 16) | (g << 8) | b;
//         }
//     }
// }

void Pipeline::ResolveMSAA()
{
    for (int y = 0; y < device._height; y ++)
    {
        for (int x = 0; x < device._width; x++)
        {
            int pixelIdx = (y * device._width + x) * 4;
            Vector4f finalColor(0, 0, 0, 0);
            
            for (int i = 0; i < 4; i ++)
            {
                finalColor = finalColor + device.msaaBuffer[pixelIdx + i].color;
            }
            unsigned int color = ColorToInt(finalColor * 0.25f);
            device._framebuffer[y * device._width + x] = color;
        }
    }
}
