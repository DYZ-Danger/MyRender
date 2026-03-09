#include "Structure.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

const float PI = acos(-1.0f);
Vector2f::Vector2f(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2f::Vector2f(const Vector2f& vec)
{
    this->x = vec.x;
    this->y = vec.y;
}

Vector2f::Vector2f()
{
    this->x = 0.0f;
    this->y = 0.0f;
}

Vector2f Vector2f::operator +(const Vector2f& vec) const
{
    return {this->x + vec.x, this->y + vec.y};
}

Vector2f Vector2f::operator -(const Vector2f& vec) const
{
    return {this->x - vec.x, this->y - vec.y};
}

Vector2f Vector2f::operator *(float k) const
{
    return {this->x * k, this->y * k};
}

Vector2f Vector2f::operator*(const Vector2f& vec) const
{
    return {this->x * vec.x, this->y * vec.y};
}

Vector2f Vector2f::operator /(float k) const
{
    return {this->x / k, this->y / k};
}

float Vector2f::Dot(const Vector2f& vec) const
{
    return this->x * vec.x + this->y * vec.y;
}

Vector2f Vector2f::Normalize() const
{
    float norm = sqrtf(x * x + y * y);
    return {this->x / norm, this->y / norm};
}

float Vector2f::Length() const
{
    return sqrtf(x * x + y * y);
}

Vector2f Vector2f::Lerp(const Vector2f& vec, float t) const
{
    return {
        this->x * (1.0f - t) + vec.x * t,
        this->y * (1.0f - t) + vec.y * t
    };
}

float Vector2f::Product(const Vector2f& vec) const
{
    return this->x * vec.y - this->y * vec.x;
}

bool Vector2f::isInsideTriangle(float px, float py, const Vector2f& v0, const Vector2f& v1, const Vector2f& v2)
{
    // 向量 0: 从 v0 指向 v1，以及从 v0 指向 P
    float c1 = Vector2f(v1.x - v0.x, v1.y - v0.y).Product(Vector2f(px - v0.x, py - v0.y));
    // 向量 1: 从 v1 指向 v2，以及从 v1 指向 P
    float c2 = Vector2f(v2.x - v1.x, v2.y - v1.y).Product(Vector2f(px - v1.x, py - v1.y));
    // 向量 2: 从 v2 指向 v0，以及从 v2 指向 P
    float c3 = Vector2f(v0.x - v2.x, v0.y - v2.y).Product(Vector2f(px - v2.x, py - v2.y));

    // 检查符号是否一致 (同时为正或同时为负，取决于三角形顶点绕序)
    // 考虑到边缘情况，包含 0 (即落在边上算在内)
    return (c1 >= -1e-2f && c2 >= -1e-2f && c3 >= -1e-2f) || (c1 <= 1e-2f && c2 <= 1e-2f && c3 <= 1e-2f);
}

Vector3f::Vector3f()
{
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
}

Vector3f::Vector3f(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3f::Vector3f(const Vector3f& vec)
{
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
}

Vector3f::Vector3f(const Vector2f& vec, float z)
{
    this->x = vec.x;
    this->y = vec.y;
    this->z = z;
}

Vector3f Vector3f::operator +(const Vector3f& vec) const
{
    return {this->x + vec.x, this->y + vec.y, this->z + vec.z};
}

Vector3f Vector3f::operator -(const Vector3f& vec) const
{
    return {this->x - vec.x, this->y - vec.y, this->z - vec.z};
}

Vector3f Vector3f::operator *(float k) const
{
    return {this->x * k, this->y * k, this->z * k};
}

Vector3f Vector3f::operator*(const Vector3f& vec) const
{
    return{this->x * vec.x, this->y * vec.y, this->z * vec.z};
}

Vector3f Vector3f::operator /(float k) const
{
    return {this->x / k, this->y / k, this->z / k};
}

float Vector3f::Dot(const Vector3f& vec) const
{
    return this->x * vec.x + this->y * vec.y + this->z * vec.z;
}

Vector3f Vector3f::Normalize() const
{
    float norm = sqrtf(x * x + y * y + z * z);
    return {this->x / norm, this->y / norm, this->z / norm};
}

float Vector3f::Length() const
{
    return sqrtf(x * x + y * y + z * z);
}

Vector3f Vector3f::Lerp(const Vector3f& vec, float t) const
{
    return {
        this->x * (1.0f - t) + vec.x * t,
        this->y * (1.0f - t) + vec.y * t,
        this->z * (1.0f - t) + vec.z * t
    };
}

Vector3f Vector3f::Product(const Vector3f& vec) const
{
    return {
        this->y * vec.z - this->z * vec.y,
        this->z * vec.x - this->x * vec.z,
        this->x * vec.y - this->y * vec.x
    };
}

Vector4f::Vector4f()
{
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
    this->w = 0.0f;
}

Vector4f::Vector4f(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4f::Vector4f(const Vector4f& vec)
{
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = vec.w;
}

Vector4f::Vector4f(const Vector3f& vec, float w)
{
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = w;
}

Vector4f Vector4f::operator +(const Vector4f& vec) const
{
    return {this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w};
}

Vector4f Vector4f::operator -(const Vector4f& vec) const
{
    return {this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w};
}

Vector4f Vector4f::operator *(float k) const
{
    return {this->x * k, this->y * k, this->z * k, this->w * k};
}

Vector4f Vector4f::operator*(const Vector4f& vec) const
{
    return {this->x * vec.x, this->y * vec.y, this->z * vec.z, this->w * vec.w};
}

Vector4f Vector4f::operator /(float k) const
{
    return {this->x / k, this->y / k, this->z / k, this->w / k};
}

float Vector4f::Dot(const Vector4f& vec) const
{
    return this->x * vec.x + this->y * vec.y + this->z * vec.z + this->w * vec.w;
}

Vector4f Vector4f::Normalize() const
{
    float norm = sqrtf(x * x + y * y + z * z + w * w);
    return {this->x / norm, this->y / norm, this->z / norm, this->w / norm};
}

float Vector4f::Length() const
{
    return sqrtf(x * x + y * y + z * z + w * w);
}

Vector4f Vector4f::Lerp(const Vector4f& vec, float t) const
{
    return {
        this->x * (1.0f - t) + vec.x * t,
        this->y * (1.0f - t) + vec.y * t,
        this->z * (1.0f - t) + vec.z * t,
        this->w * (1.0f - t) + vec.w * t
    };
}

Matrix4f::Matrix4f()
{
    fill_n(&element[0][0], 16, 0.0f);
}

Matrix4f::Matrix4f(const Matrix4f& mat)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            element[i][j] = mat.element[i][j];
        }
    }
}

Matrix4f::Matrix4f(float matValue[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            element[i][j] = matValue[i][j];
        }
    }
}

float Matrix4f::operator()(int i, int j) const
{
    return element[i][j];
}

Matrix4f Matrix4f::operator*(const Matrix4f& mat) const
{
    Matrix4f result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            float sum = 0.0f;
            for (int k = 0; k < 4; k++)
            {
                sum += this->operator()(i, k) * mat(k, j);
            }
            result.set(i, j, sum);
        }
    }
    return result;
}

Matrix4f Matrix4f::operator+(const Matrix4f& mat) const
{
    Matrix4f result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.element[i][j] = this->operator()(i, j) + mat(i, j);
        }
    }
    return result;
}

Matrix4f Matrix4f::operator-(const Matrix4f& mat) const
{
    Matrix4f result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.element[i][j] = this->operator()(i, j) - mat(i, j);
        }
    }
    return result;
}

Matrix4f Matrix4f::operator*(float k) const
{
    Matrix4f result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.element[i][j] = this->operator()(i, j) * k;
        }
    }
    return result;
}

Vector4f Matrix4f::rowVector(int i) const
{
    Vector4f vec;
    vec.x = element[i][0];
    vec.y = element[i][1];
    vec.z = element[i][2];
    vec.w = element[i][3];
    return vec;
}

Vector4f Matrix4f::colVector(int i) const
{
    Vector4f vec;
    vec.x = element[0][i];
    vec.y = element[1][i];
    vec.z = element[2][i];
    vec.w = element[3][i];
    return vec;
}

Vector4f Matrix4f::operator*(const Vector4f& vec) const
{
    Vector4f result;
    result.x = this->rowVector(0).Dot(vec);
    result.y = this->rowVector(1).Dot(vec);
    result.z = this->rowVector(2).Dot(vec);
    result.w = this->rowVector(3).Dot(vec);
    return result;
}

Matrix4f Matrix4f::Transpose() const
{
    Matrix4f result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.element[i][j] = element[j][i];
        }
    }
    return result;
}

void Matrix4f::set(int i, int j, float value)
{
    element[i][j] = value;
}

Matrix4f Matrix4f::Identity()
{
    Matrix4f result;
    for (int i = 0; i < 4; i++)
    {
        result.element[i][i] = 1.0f;
    }
    return result;
}

Matrix4f Matrix4f::Translation(const Vector3f& translation)
{
    Matrix4f result = Identity();
    result.element[0][3] = translation.x;
    result.element[1][3] = translation.y;
    result.element[2][3] = translation.z;
    return result;
}

Matrix4f Matrix4f::Scale(const Vector3f& scale)
{
    Matrix4f result = Identity();
    result.element[0][0] = scale.x;
    result.element[1][1] = scale.y;
    result.element[2][2] = scale.z;
    return result;
}

Matrix4f Matrix4f::Rotation(float angle, char axis)
{
    Matrix4f result = Identity();
    float radian = angle * (PI / 180.0f);
    float sine = sin(radian);
    float cosine = cos(radian);
    switch (axis)
    {
        case 'x':
            result.element[1][1] = cosine;
            result.element[1][2] = -sine;
            result.element[2][1] = sine;
            result.element[2][2] = cosine;
            break;
        case 'y':
            result.element[0][0] = cosine;
            result.element[0][2] = sine;
            result.element[2][0] = -sine;
            result.element[2][2] = cosine;
            break;
        case 'z':
            result.element[0][0] = cosine;
            result.element[0][1] = -sine;
            result.element[1][0] = sine;
            result.element[1][1] = cosine;
            break;
        default:
            break;
    }
    return result;
}

Matrix4f Matrix4f::LookAt(const Vector3f& eye, const Vector3f& target, const Vector3f& up)
{
    Vector3f z = (eye - target).Normalize();
    Vector3f x = z.Product(up).Normalize();
    Vector3f y = z.Product(x).Normalize();
    
    Matrix4f result = Identity();
    result.element[0][0] = x.x;
    result.element[0][1] = x.y;
    result.element[0][2] = x.z;
    result.element[1][0] = y.x;
    result.element[1][1] = y.y;
    result.element[1][2] = y.z;
    result.element[2][0] = -z.x;
    result.element[2][1] = -z.y;
    result.element[2][2] = -z.z;
    Matrix4f trans = Matrix4f::Translation(Vector3f(-eye.x, -eye.y, -eye.z));
    return result * trans;
}

Matrix4f Matrix4f::Perspective(float fov, float aspect, float near, float far)
{
    Matrix4f result;
    float tanHalfFov = tan(fov * 0.5f * (PI / 180.0f));
    result.element[0][0] = 1.0f / (aspect * tanHalfFov);
    result.element[1][1] = 1.0f / (tanHalfFov);
    result.element[2][2] = -(far + near) / (far - near);
    result.element[2][3] = -(2.0f * far * near) / (far - near);
    result.element[3][2] = -1.0f;
    return result;
}

Vertex::Vertex()
{
    this->color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
    this->position = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
}

Vertex::Vertex(const Vertex& vertex)
{
    this->color = vertex.color;
    this->position = vertex.position;
    this->normal = vertex.normal;
    this->texcoord = vertex.texcoord;
}

Vertex::Vertex(const Vector4f& position, const Vector4f& color, const Vector3f& normal, const Vector2f& texcoord)
{
    this->color = color;
    this->position = position;
    this->normal = normal;
    this->texcoord = texcoord;
}

Mesh::Mesh(const Mesh& mesh)
{
    this->vertices = mesh.vertices;
    this->indices = mesh.indices;
}

void Mesh::SetIndices(unsigned int* _indices, unsigned int numIndices)
{
    this->indices.assign(_indices, _indices + numIndices);
}

void Mesh::SetVertices(Vertex* _vertices, unsigned int numVertices)
{
    this->vertices.assign(_vertices, _vertices + numVertices);
}

Mesh Mesh::cube(float size)
{
    Mesh result;
    float s = size / 2.0f;
    
    Vector3f nFront(0.0f, 0.0f, 1.0f), nBack(0.0f, 0.0f, -1.0f);
    Vector3f nLeft(-1.0f, 0.0f, 0.0f), nRight(1.0f, 0.0f, 0.0f);
    Vector3f nTop(0.0f, 1.0f, 0.0f),   nBottom(0.0f, -1.0f, 0.0f);
    
    Vector2f uv1 = {0.0f, 0.0f};
    Vector2f uv2 = {1.0f, 0.0f};
    Vector2f uv3 = {1.0f, 1.0f};
    Vector2f uv4 = {0.0f, 1.0f};
    
    Vertex v[] = {
        // // 前面 (Z = s) - 保持你原来的颜色：红、绿、蓝、白
        // { { -s, -s,  s, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, nFront, uv1 }, // 0
        // { {  s, -s,  s, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, nFront, uv2 }, // 1
        // { {  s,  s,  s, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, nFront, uv3 }, // 2
        // { { -s,  s,  s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nFront, uv4 }, // 3
        //
        // // 后面 (Z = -s) - 保持你原来的颜色：黄、品红、青、灰
        // { { -s, -s, -s, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, nBack, uv1 },  // 4
        // { {  s, -s, -s, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, nBack, uv2 },  // 5
        // { {  s,  s, -s, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, nBack, uv3 },  // 6
        // { { -s,  s, -s, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }, nBack, uv4 },  // 7
        //
        // // 左面 (X = -s)
        // { { -s, -s, -s, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, nLeft, uv1 },  // 8 (同4)
        // { { -s, -s,  s, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, nLeft, uv2 },  // 9 (同0)
        // { { -s,  s,  s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nLeft, uv3 },  // 10(同3)
        // { { -s,  s, -s, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }, nLeft, uv4 },  // 11(同7)
        //
        // // 右面 (X = s)
        // { {  s, -s,  s, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, nRight, uv1 }, // 12(同1)
        // { {  s, -s, -s, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, nRight, uv2 }, // 13(同5)
        // { {  s,  s, -s, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, nRight, uv3 }, // 14(同6)
        // { {  s,  s,  s, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, nRight, uv4 }, // 15(同2)
        //
        // // 上面 (Y = s)
        // { { -s,  s,  s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nTop, uv1 },   // 16(同3)
        // { {  s,  s,  s, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, nTop, uv2 },   // 17(同2)
        // { {  s,  s, -s, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, nTop, uv3 },   // 18(同6)
        // { { -s,  s, -s, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }, nTop, uv4 },   // 19(同7)
        //
        // // 下面 (Y = -s)
        // { { -s, -s, -s, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, nBottom, uv1 },// 20(同4)
        // { {  s, -s, -s, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, nBottom, uv2 },// 21(同5)
        // { {  s, -s,  s, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, nBottom, uv3 },// 22(同1)
        // { { -s, -s,  s, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, nBottom, uv4 } // 23(同0)
        // 前面 (Z = s)
        { { -s, -s,  s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nFront, uv1 }, // 0
        { {  s, -s,  s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nFront, uv2 }, // 1
        { {  s,  s,  s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nFront, uv3 }, // 2
        { { -s,  s,  s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nFront, uv4 }, // 3
        
        // 后面 (Z = -s)
        { { -s, -s, -s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nBack, uv1 },  // 4
        { {  s, -s, -s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nBack, uv2 },  // 5
        { {  s,  s, -s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nBack, uv3 },  // 6
        { { -s,  s, -s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nBack, uv4 },  // 7
        
        // 左面 (X = -s)
        { { -s, -s, -s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nLeft, uv1 },  // 8
        { { -s, -s,  s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nLeft, uv2 },  // 9
        { { -s,  s,  s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nLeft, uv3 },  // 10
        { { -s,  s, -s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nLeft, uv4 },  // 11
        
        // 右面 (X = s)
        { {  s, -s,  s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nRight, uv1 }, // 12
        { {  s, -s, -s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nRight, uv2 }, // 13
        { {  s,  s, -s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nRight, uv3 }, // 14
        { {  s,  s,  s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nRight, uv4 }, // 15
        
        // 上面 (Y = s)
        { { -s,  s,  s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nTop, uv1 },   // 16
        { {  s,  s,  s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nTop, uv2 },   // 17
        { {  s,  s, -s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nTop, uv3 },   // 18
        { { -s,  s, -s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nTop, uv4 },   // 19
        
        // 下面 (Y = -s)
        { { -s, -s, -s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nBottom, uv1 },// 20
        { {  s, -s, -s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nBottom, uv2 },// 21
        { {  s, -s,  s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nBottom, uv3 },// 22
        { { -s, -s,  s, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, nBottom, uv4 } // 23
    };
    
    
    unsigned int i[] = {
        0, 1, 2,    0, 2, 3,    // 前
        4, 6, 5,    4, 7, 6,    // 后
        8, 9, 10,   8, 10, 11,  // 左
        12, 13, 14, 12, 14, 15, // 右
        16, 17, 18, 16, 18, 19, // 上
        20, 21, 22, 20, 22, 23  // 下
    };
    result.SetVertices(v, 24);
    result.SetIndices(i, 36);
    return result;
}

void Mesh::LoadObj(const std::string& filename) {
    std::vector<Vector3f> temp_positions;
    std::vector<Vector2f> temp_uvs;
    std::vector<Vector3f> temp_normals;

    // 清空现有数据，防止多次加载叠加
    this->vertices.clear();
    this->indices.clear();

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            Vector3f pos;
            ss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);
        }
        else if (prefix == "vt") {
            Vector2f uv;
            ss >> uv.x >> uv.y;
            temp_uvs.push_back(uv);
        }
        else if (prefix == "vn") {
            Vector3f normal;
            ss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        else if (prefix == "f") {
            std::string vertexStr;
            std::vector<Vertex> faceVertices;

            while (ss >> vertexStr) {
                int vIdx = 0, vtIdx = 0, vnIdx = 0;
                const char* s = vertexStr.c_str();

                // 健壮的 sscanf_s 匹配逻辑
                // 1. 尝试匹配 v/vt/vn
                if (sscanf_s(s, "%d/%d/%d", &vIdx, &vtIdx, &vnIdx) == 3) {
                    // 匹配成功
                }
                // 2. 尝试匹配 v//vn (无纹理坐标)
                else if (sscanf_s(s, "%d//%d", &vIdx, &vnIdx) == 2) {
                    vtIdx = 0;
                }
                // 3. 尝试匹配 v/vt (无采样法线)
                else if (sscanf_s(s, "%d/%d", &vIdx, &vtIdx) == 2) {
                    vnIdx = 0;
                }
                // 4. 仅有顶点 v
                else if (sscanf_s(s, "%d", &vIdx) == 1) {
                    vtIdx = 0;
                    vnIdx = 0;
                }

                Vertex v;
                // 默认值初始化，防止光照/贴图完全失效
                v.color = { 1.0f, 1.0f, 1.0f, 1.0f };
                v.normal = { 0.0f, 1.0f, 0.0f }; // 默认法线向上
                v.texcoord = { 0.0f, 0.0f };

                // 顶点坐标
                if (vIdx > 0 && vIdx <= (int)temp_positions.size()) 
                    v.position = Vector4f(temp_positions[vIdx - 1], 1.0f);
                
                // 纹理坐标 (处理 V 轴翻转)
                if (vtIdx > 0 && vtIdx <= (int)temp_uvs.size()) {
                    v.texcoord = temp_uvs[vtIdx - 1];
                    // 如果贴图还是反的，请取消下面这行的注释：
                    // v.texcoord.y = 1.0f - v.texcoord.y; 
                }
                
                // 法线
                if (vnIdx > 0 && vnIdx <= (int)temp_normals.size()) 
                    v.normal = temp_normals[vnIdx - 1];

                faceVertices.push_back(v);
            }

            // 多边形拆分 (Triangle Fan)
            for (size_t i = 1; i < faceVertices.size() - 1; ++i) {
                this->vertices.push_back(faceVertices[0]);
                this->vertices.push_back(faceVertices[i]);
                this->vertices.push_back(faceVertices[i + 1]);
                
                this->indices.push_back((unsigned int)this->vertices.size() - 3);
                this->indices.push_back((unsigned int)this->vertices.size() - 2);
                this->indices.push_back((unsigned int)this->vertices.size() - 1);
            }
        }
    }
    file.close();
}

void VerToFrag::operator +=(const VerToFrag& vert)
{
    this->projection_position = this->projection_position + vert.projection_position;
    this->screen_position = this->screen_position + vert.screen_position;
    this->color = this->color + vert.color;
    this->oneDivZ = this->oneDivZ + vert.oneDivZ;
    this->uvOverZ = this->uvOverZ + vert.uvOverZ;
    this->normal = this->normal + vert.normal;
}

VerToFrag VerToFrag::operator *(float k) const
{
    VerToFrag result;
    result.projection_position = this->projection_position * k;
    result.screen_position = this->screen_position * k;
    result.color = this->color * k;
    result.oneDivZ = this->oneDivZ * k;
    result.uvOverZ = this->uvOverZ * k;
    result.normal = this->normal * k;
    return result;
}

VerToFrag VerToFrag::operator -(const VerToFrag& vert) const
{
    VerToFrag result;
    result.projection_position = this->projection_position - vert.projection_position;
    result.screen_position = this->screen_position - vert.screen_position;
    result.color = this->color - vert.color;
    result.oneDivZ = this->oneDivZ - vert.oneDivZ;
    result.uvOverZ = this->uvOverZ - vert.uvOverZ;
    result.normal = this->normal - vert.normal;
    return result;
}

VerToFrag VerToFrag::operator+(const VerToFrag& vert) const
{
    VerToFrag result;
    result.projection_position = this->projection_position + vert.projection_position;
    result.screen_position = this->screen_position + vert.screen_position;
    result.color = this->color + vert.color;
    result.oneDivZ = this->oneDivZ + vert.oneDivZ;
    result.uvOverZ = this->uvOverZ + vert.uvOverZ;
    result.normal = this->normal + vert.normal;
    return result;
}

