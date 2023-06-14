#pragma once

#include <DirectXMath.h>
using namespace DirectX;

// çáéZån

inline const XMFLOAT3 operator+(const XMFLOAT3& F1, const XMFLOAT3& F2)
{
    const XMFLOAT3 F = {
        F1.x + F2.x,
        F1.y + F2.y,
        F1.z + F2.z,
    };

    return F;
}
inline const XMFLOAT3 operator+(const XMFLOAT3& F, const float& f)
{
    const XMFLOAT3 result = {
        F.x + f,
        F.y + f,
        F.z + f,
    };

    return result;
}

inline const XMFLOAT3 operator-(const XMFLOAT3& F1, const XMFLOAT3& F2)
{
    const XMFLOAT3 F = {
        F1.x - F2.x,
        F1.y - F2.y,
        F1.z - F2.z,
    };

    return F;
}
inline const XMFLOAT3 operator-(const XMFLOAT3& F, const float& f)
{
    const XMFLOAT3 result = {
        F.x - f,
        F.y - f,
        F.z - f,
    };

    return result;
}

inline const XMFLOAT3 operator*(const XMFLOAT3& F1, const XMFLOAT3& F2)
{
    const XMFLOAT3 F = {
        F1.x * F2.x,
        F1.y * F2.y,
        F1.z * F2.z,
    };

    return F;
}
inline const XMFLOAT3 operator*(const XMFLOAT3& F, const float& f)
{
    const XMFLOAT3 result = {
        F.x * f,
        F.y * f,
        F.z * f,
    };

    return result;
}

inline const XMFLOAT3 operator/(const XMFLOAT3& F1, const XMFLOAT3& F2)
{
    const XMFLOAT3 F = {
        F1.x / F2.x,
        F1.y / F2.y,
        F1.z / F2.z,
    };

    return F;
}
inline const XMFLOAT3 operator/(const XMFLOAT3& F, const float& f)
{
    const XMFLOAT3 result = {
        F.x / f,
        F.y / f,
        F.z / f,
    };

    return result;
}


// çáéZë„ì¸ån

inline const XMFLOAT3& operator+=(XMFLOAT3& F1, const XMFLOAT3& F2)
{
    F1.x += F2.x;
    F1.y += F2.y;
    F1.z += F2.z;

    return F1;
}
inline const XMFLOAT3& operator+=(XMFLOAT3& F, const float& f)
{
    F.x += f;
    F.y += f;
    F.z += f;

    return F;
}

inline const XMFLOAT3& operator-=(XMFLOAT3& F1, const XMFLOAT3& F2)
{
    F1.x -= F2.x;
    F1.y -= F2.y;
    F1.z -= F2.z;

    return F1;
}
inline const XMFLOAT3& operator-=(XMFLOAT3& F, const float& f)
{
    F.x -= f;
    F.y -= f;
    F.z -= f;

    return F;
}

inline const XMFLOAT3& operator*=(XMFLOAT3& F1, const XMFLOAT3& F2)
{
    F1.x *= F2.x;
    F1.y *= F2.y;
    F1.z *= F2.z;

    return F1;
}
inline const XMFLOAT3& operator*=(XMFLOAT3& F, const float& f)
{
    F.x *= f;
    F.y *= f;
    F.z *= f;

    return F;
}

inline const XMFLOAT3& operator/=(XMFLOAT3& F1, const XMFLOAT3& F2)
{
    F1.x /= F2.x;
    F1.y /= F2.y;
    F1.z /= F2.z;

    return F1;
}
inline const XMFLOAT3& operator/=(XMFLOAT3& F, const float& f)
{
    F.x /= f;
    F.y /= f;
    F.z /= f;

    return F;
}
