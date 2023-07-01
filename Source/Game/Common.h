#pragma once

#include <DirectXMath.h>
#include "OperatorXMFLOAT3.h"

// �}�N���萔
// const�s�v�̖ڈ�
#define NO_CONST 


// �֐��|�C���^
// ���W�A���ϊ��֐��|�C���^
extern float (* const ToRadian)(float); 


// �|�C���^�����֐�
template <typename T>
inline void SafeDelete(T*& p)
{
    if (!p) return;

    delete p;
    p = nullptr;
}

// XMFLOAT3���m�̓��ς��v�Z
inline const float XMFloat3Dot(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	return ( (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) );
}

// XMFLOAT3�̐��K���i�P�ʃx�N�g�����E�W�����j
inline const DirectX::XMFLOAT3 XMFloat3Normalize(const DirectX::XMFLOAT3& vec)
{
	const float length = sqrtf(XMFloat3Dot(vec, vec));

	if (length <= 0.0f) return DirectX::XMFLOAT3(0,0,0);

	return (vec / length);
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

// XMFLOAT3�̐�Βl�����߂�
inline const DirectX::XMFLOAT3 XMFloat3Abs(const DirectX::XMFLOAT3& v)
{
	return DirectX::XMFLOAT3(fabsf(v.x), fabsf(v.y), fabsf(v.z));
}