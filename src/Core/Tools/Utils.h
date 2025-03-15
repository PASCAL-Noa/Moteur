#pragma once

#include <DirectXMath.h>

class CameraComponent;
class Utils
{
public:
    static void PrintMatrix(const DirectX::XMFLOAT4X4& matrix);
    static void PrintVector(const char* name, const DirectX::XMFLOAT3& vec);
    static DirectX::XMFLOAT3 Normalize(const DirectX::XMFLOAT3& vec);
	static float Lerp(float a, float b, float t);
    static std::wstring ConvertToWideString(const std::string& str);
    
    struct Vector3f
    {
        float x, y, z;
        
        Vector3f() : x(0.0f), y(0.0f), z(0.0f) {}
        Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}

        Vector3f operator+(const Vector3f& other) const {return Vector3f(x + other.x, y + other.y, z + other.z);}
        
        Vector3f operator-(const Vector3f& other) const {return Vector3f(x - other.x, y - other.y, z - other.z);}
        
        Vector3f operator*(float scalar) const {return Vector3f(x * scalar, y * scalar, z * scalar);}
        
        Vector3f operator/(float scalar) const {return (scalar != 0.0f) ? Vector3f(x / scalar, y / scalar, z / scalar) : Vector3f();}
        
        Vector3f& operator+=(const Vector3f& other) {x += other.x; y += other.y; z += other.z; return *this;}
        
        Vector3f& operator-=(const Vector3f& other) {x -= other.x; y -= other.y; z -= other.z; return *this;}
        
        Vector3f& operator*=(float scalar) {x *= scalar; y *= scalar; z *= scalar; return *this;}
        
        Vector3f& operator/=(float scalar)
        {
            if (scalar != 0.0f)
            {
                x /= scalar; y /= scalar; z /= scalar;
            }
            return *this;
        }

        float Magnitude() const {return sqrt(x * x + y * y + z * z);}

        Vector3f Normalize() const
        {
            float mag = Magnitude();
            return (mag > 0.0f) ? *this / mag : Vector3f();
        }
    };

    struct Vector3i
    {
        int x, y, z;

        Vector3i() : x(0), y(0), z(0) {}
        Vector3i(int x, int y, int z) : x(x), y(y), z(z) {}

        Vector3i operator+(const Vector3i& other) const { return Vector3i(x + other.x, y + other.y, z + other.z); }
        Vector3i operator-(const Vector3i& other) const { return Vector3i(x - other.x, y - other.y, z - other.z); }
        Vector3i operator*(int scalar) const { return Vector3i(x * scalar, y * scalar, z * scalar); }

        bool operator==(const Vector3i& other) const { return x == other.x && y == other.y && z == other.z; }
        bool operator!=(const Vector3i& other) const { return !(*this == other); }

        struct Hash
        {
            std::size_t operator()(const Vector3i& v) const
            {
                return ((std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1)) >> 1) ^ (std::hash<int>()(v.z) << 1);
            }
        };
    };
    static void ScreenToWorldRay(const DirectX::XMFLOAT2& screenPos, const CameraComponent& camera,
                          DirectX::XMFLOAT3& rayOrigin, DirectX::XMFLOAT3& rayDirection);
};
