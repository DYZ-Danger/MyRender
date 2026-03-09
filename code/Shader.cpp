#include "Shader.h"

#include "Pipeline.h"

VerToFrag SimpleShader::VertexShader(const Vertex& vex)
{
    VerToFrag result;
    result.projection_position = ProjectionMatrix * ViewMatrix * ModelMatrix * vex.position;
    
    float w = result.projection_position.w;
    result.oneDivZ = 1.0f / w;
    result.uvOverZ = vex.texcoord * result.oneDivZ;
    
    Vector4f n = (ModelMatrix * Vector4f(vex.normal, 0.0f));
     result.normal = Vector3f(n.x, n.y, n.z).Normalize();
     // if (diff > 0.1f) 
     //     finalcolor = Vector3f(1.0f, 1.0f, 1.0f); // 受光面纯白
     // else 
     //     finalcolor = Vector3f(0.1f, 0.1f, 0.1f); // 背光面深灰
     result.color = vex.color;
    
    // result.color = Vector4f(abs(vex.normal.x), abs(vex.normal.y), abs(vex.normal.z), 1.0f);
    
    return result;
}

Vector4f SimpleShader::FragmentShader(const VerToFrag& vex)
{
    // 1. 极小值保护：如果 oneDivZ 太小，说明该像素极度靠近裁剪面或发生了数值错误
    // 此时直接放弃高精度插值，防止 UV 爆炸
    float min_oneDivZ = 1e-6f;
    Vector2f realUV(0, 0);
    if (std::abs(vex.oneDivZ) > min_oneDivZ) {
        realUV = vex.uvOverZ / vex.oneDivZ;
    }

    // 2. 采样保护
    Vector4f texColor = mytexture->Sample(realUV);
    Vector3f baseColor = {texColor.x, texColor.y, texColor.z};

    // 3. 法线归一化防闪烁
    // 裁剪边缘插值出的法线长度可能接近 0，导致 Normalize() 产生 NaN 导致颜色跳变
    Vector3f N;
    float n_len = vex.normal.Length();
    if (n_len < 1e-5f) {
        N = Vector3f(0, 0, 1); 
    } else {
        N = vex.normal.Normalize();
    }
    
    // 4. 光照计算
    Vector3f Lightdir = Vector3f(1.0f, 2.0f, -1.0f).Normalize();
    Vector3f Lightcolor = {1.0f, 1.0f, 1.0f};
     
    float ambientStrength = 0.2f;
    Vector3f ambient = baseColor * ambientStrength;
    
    // 5. 漫反射防溢出：严格限制在 [0, 1]
    float dotNL = N.Dot(Lightdir);
    float diff = (std::max)(0.0f, (std::min)(1.0f, dotNL)); 
    Vector3f diffuse = baseColor * Lightcolor * diff;
    
    // 6. 最终合成
    Vector3f finalcolor = ambient + diffuse;
    
    // 7. 颜色钳位：这是解决黑白线条的关键
    // 将 0-1 的浮点数严格限制，防止转为字节时发生环绕(Wrap)
    return Vector4f(
        (std::max)(0.0f, (std::min)(1.0f, finalcolor.x)),
        (std::max)(0.0f, (std::min)(1.0f, finalcolor.y)),
        (std::max)(0.0f, (std::min)(1.0f, finalcolor.z)),
        1.0f
    );
    //return Vector4f(baseColor, 1);
}
