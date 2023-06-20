#pragma once

#include <DirectXMath.h>

// �}�N���萔
// const�s�v�̖ڈ�
#define NO_CONST 


// �֐��|�C���^
// ���W�A���ϊ��֐��|�C���^
extern float (* const ToRadian)(float); 


// inline�֐�
// �|�C���^�����֐�
template <typename T>
inline void SafeDelete(T*& p)
{
    if (!p) return;

    delete p;
    p = nullptr;
}

// XMFLOAT3�̐��K���i�P�ʃx�N�g�����E�W�����j
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

// XMFLOAT3���m�̓��ς��v�Z
inline const float XMFloat3Dot(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	const float dot = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	return dot;
}

// XMFLOAT3���m�̊O�ς��v�Z(���Ԃɒ���)
inline const DirectX::XMFLOAT3 XMFloat3Cross(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	const DirectX::XMFLOAT3 cross = {
		(v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x),
	};			
	return cross;
}