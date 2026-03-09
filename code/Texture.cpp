#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"

#include <cmath>

#include "Pipeline.h"

Texture::Texture(int w = 256, int h = 256)
    :width(w), height(h)
{
    // data.resize(width * height);
    // for (int y = 0; y < height; y++)
    // {
    //     for (int x = 0; x < width; x++)
    //     {
    //         bool isWhite = ((x / 32 + y / 32) % 2) == 0;
    //         data[y * width + x] = isWhite ? 0xFFFFFFFF : 0xFF000000;
    //     }
    // }
}

Vector4f Texture::Sample(Vector2f uv)
{
    if (data.empty()) return Vector4f(1.0f, 0.0f, 1.0f, 1.0f);
    
    float u = uv.x - floor(uv.x);
    float v = uv.y - floor(uv.y);
    
    float texX = u * (width - 1);
    float texY = v * (height - 1);
    
    int x0 = static_cast<int>(std::floor(texX));
    int y0 = static_cast<int>(std::floor(texY));
    float dx = texX - x0;
    float dy = texY - y0;
    
    int x1 = (std::min)(x0 + 1, width - 1);
    int y1 = (std::min)(y0 + 1, height - 1);
    
    auto getVecColor = [&](int x, int y) {
        unsigned int color = data[static_cast<size_t>(y) * width + x];
        return Vector4f(
            ((color >> 16) & 0xFF) / 255.0f,
            ((color >> 8) & 0xFF) / 255.0f,
            (color & 0xFF) / 255.0f,
            1.0f
        );
    };
    
    Vector4f c00 = getVecColor(x0, y0);
    Vector4f c10 = getVecColor(x1, y0);
    Vector4f c01 = getVecColor(x0, y1);
    Vector4f c11 = getVecColor(x1, y1);
    
    Vector4f top = c00 * (1.0f - dx) + c10 * dx;
    Vector4f bottom = c01 * (1.0f - dx) + c11 * dx;
    
    return top * (1.0f - dy) + bottom * dy;
}

void Texture::LoadTexture(const std::string& path)
{
    int channels;
    // 强制加载为 4 通道 (RGBA)
    //stbi_set_flip_vertically_on_load(true);
    unsigned char* img_data = stbi_load(path.c_str(), &width, &height, &channels, 4);

    if (img_data) {
        data.clear();
        data.resize(width * height);
        
        for (int i = 0; i < width * height; i++) {
            unsigned char r = img_data[i * 4 + 0];
            unsigned char g = img_data[i * 4 + 1];
            unsigned char b = img_data[i * 4 + 2];
            unsigned char a = img_data[i * 4 + 3];
            
            // 拼接成 0xAARRGGBB 格式（取决于你 Sample 函数里的移位逻辑）
            data[i] = (a << 24) | (r << 16) | (g << 8) | b;
        }
        stbi_image_free(img_data);
    } else {
        std::string reason = stbi_failure_reason();
        std::string error_msg = "load failed [" + path + "], reason: " + reason + "\n";
        OutputDebugStringA(error_msg.c_str());
    }
}
