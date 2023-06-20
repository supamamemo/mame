#pragma once

#include <DirectXMath.h>

// マクロ定数
// const不要の目印
#define NO_CONST 


// 関数ポインタ
// ラジアン変換関数ポインタ
extern float (* const ToRadian)(float); 


// inline関数
// ポインタ消去関数
template <typename T>
inline void SafeDelete(T*& p)
{
    if (!p) return;

    delete p;
    p = nullptr;
}

// XMFLOAT3の正規化（単位ベクトル化・標準化）
inline const DirectX::XMFLOAT3 XMFLoat3Normalize(const DirectX::XMFLOAT3& vec)
{
	const float length = sqrtf(
		(vec.x * vec.x) + 
		(vec.y * vec.y) + 
		(vec.z * vec.z)
	);

	if (length <= 0.0f) return DirectX::XMFLOAT3(0, 0, 0);

	const DirectX::XMFLOAT3 normalize = {
		(vec.x / length), 
		(vec.y / length), 
		(vec.z / length)
	};

	return normalize;
}

// XMFLOAT3同士の内積を計算
inline const float XMFloat3Dot(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	const float dot = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	return dot;
}

// XMFLOAT3同士の外積を計算(順番に注意)
inline const DirectX::XMFLOAT3 XMFloat3Cross(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	const DirectX::XMFLOAT3 cross = {
		(v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x),
	};			
	return cross;
}