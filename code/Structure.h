#pragma once
#include <string>
#include <vector>
class Vector2f
{
    public:
        float x, y;
        Vector2f();
        Vector2f(float x, float y);
        Vector2f(const Vector2f& vec);
        ~Vector2f() = default;
    
        Vector2f operator +(const Vector2f& vec) const;
        Vector2f operator -(const Vector2f& vec) const;
        
        Vector2f operator *(float k) const;
        Vector2f operator *(const Vector2f& vec) const;
        Vector2f operator /(float k) const;
    
        float Dot(const Vector2f& vec) const;
        float Product(const Vector2f& vec) const;
        Vector2f Normalize() const;
        float Length() const;
        Vector2f Lerp(const Vector2f& vec, float t) const;
        static bool isInsideTriangle(float px, float py, const Vector2f& v0, const Vector2f& v1, const Vector2f& v2);
};

class Vector3f
{
    public:
        float x, y, z;
        Vector3f();
        Vector3f(float x, float y, float z);
        Vector3f(const Vector3f& vec);
        Vector3f(const Vector2f& vec, float z);
        ~Vector3f() = default;
    
        Vector3f operator +(const Vector3f& vec) const;
        Vector3f operator -(const Vector3f& vec) const;
        
        Vector3f operator *(float k) const;
        Vector3f operator /(float k) const;
        Vector3f operator *(const Vector3f& vec) const;
        
        float Dot(const Vector3f& vec) const;
        Vector3f Normalize() const;
        float Length() const;
        Vector3f Lerp(const Vector3f& vec, float t) const;
        Vector3f Product(const Vector3f& vec) const;
};
class Vector4f
{
public:
    float x, y, z, w;
    Vector4f();
    Vector4f(float x, float y, float z, float w);
    Vector4f(const Vector4f& vec);
    Vector4f(const Vector3f& vec, float w);
    ~Vector4f() = default;
    
    Vector4f operator +(const Vector4f& vec) const;
    Vector4f operator -(const Vector4f& vec) const;
    
    Vector4f operator *(float k) const;
    Vector4f operator *(const Vector4f& vec) const;
    Vector4f operator /(float k) const;
    
    float Dot(const Vector4f& vec) const;
    Vector4f Normalize() const;
    float Length() const;
    Vector4f Lerp(const Vector4f& vec, float t) const;
};

class Matrix4f
{
    public:
        float operator ()(int i, int j) const;
        void set(int i, int j, float value);
        
        Matrix4f();
        ~Matrix4f() = default;
        Matrix4f(const Matrix4f& mat);
        Matrix4f(float matValue[4][4]);
        
        Matrix4f operator +(const Matrix4f& mat) const;
        Matrix4f operator -(const Matrix4f& mat) const;
        Matrix4f operator *(float k) const;
        Matrix4f operator *(const Matrix4f& mat) const;
        Vector4f operator *(const Vector4f& vec) const;
        
        Matrix4f Transpose() const;
        Vector4f rowVector(int i) const;
        Vector4f colVector(int i) const;
        static Matrix4f Identity();
    
        static Matrix4f Translation(const Vector3f& translation);
        static Matrix4f Scale(const Vector3f& scale);
        static Matrix4f Rotation(float angle, char axis);
        static Matrix4f LookAt(const Vector3f& eye, const Vector3f& target, const Vector3f& up);
        static Matrix4f Perspective(float fov, float aspect, float near, float far);
    private:
        float element[4][4];
};

class Vertex
{
    public:
        Vertex();
        Vertex(const Vector4f& position, const Vector4f& color, const Vector3f& normal, const Vector2f& texcoord);
        Vertex(const Vertex& vertex);
        ~Vertex() = default;
        
        Vector4f position;
        Vector4f color;
        Vector3f normal;
        Vector2f texcoord;
};

class Mesh
{
    public:
    Mesh() = default;
    Mesh(const Mesh& mesh);
    ~Mesh() = default;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    void SetVertices(Vertex* vertices, unsigned int numVertices);
    void SetIndices(unsigned int* indices, unsigned int numIndices);
    void LoadObj(const std::string& filename);
    
    static Mesh cube(float size);
};

class VerToFrag
{
    public:
    VerToFrag() = default;
    ~VerToFrag() = default;
    
    Vector4f projection_position;
    Vector2f screen_position;
    Vector4f color;
    Vector3f normal;
    Vector2f texcoord;
    
    float oneDivZ;
    Vector2f uvOverZ;
    
    void operator +=(const VerToFrag& vert);
    VerToFrag operator -(const VerToFrag& vert) const;
    VerToFrag operator *(float k) const;
    VerToFrag operator +(const VerToFrag& vert) const;
};