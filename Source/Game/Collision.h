#pragma once

#include <DirectXMath.h>

//#include "Graphics/Graphics.h"
#include "../Mame/Graphics/Model.h"
#include "Common.h"


// �q�b�g����
struct HitResult
{
    DirectX::XMFLOAT3 position      = { 0,0,0 }; // ���C�ƃ|���S���̌�_
    DirectX::XMFLOAT3 normal        = { 0,0,0 }; // �Փ˂����|���S���̖@���x�N�g��
    float             distance      = 0.0f;      // ���C�̎n�_�����_�܂ł̋���
    int               materialIndex = -1;        // �Փ˂����|���S���̃}�e���A���ԍ�
    DirectX::XMFLOAT3 rotation      = { 0,0,0 }; // ��]��
};

// �����Z�����Ȃ�����XMFLOAT2�B���Ȃ炱���AABB�ł͂Ȃ��i�΁j
struct AABB
{
    DirectX::XMFLOAT2 center;   // ���S�_�̍��W
    DirectX::XMFLOAT2 range;    // ���a
};


// �R���W����
class Collision
{
public:
    // ���Ƌ��̌�������
    static bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA, const float& radiusA,
        const DirectX::XMFLOAT3& positionB, const float& radiusB,
        DirectX::XMFLOAT3& outPositionB
    );

    // �~���Ɖ~���̌�������
    static bool IntersectCylinderVsCylinder(
        const DirectX::XMFLOAT3& positionA,
        const float& radiusA, 
        const float& heightA,
        const DirectX::XMFLOAT3& positionB,
        const float& radiusB, 
        const float& heightB,
        DirectX::XMFLOAT3& outPositionB
    );

    // ���Ɖ~���̌�������
    static bool IntersectSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition,
        const float& sphereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        const float& cylinderRadius,
        const float& cylinderHeight,
        DirectX::XMFLOAT3& outCylinderPosition
    );

    // ���C�ƃ��f���̌�������
    static bool IntersectRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );

    // AABB�Ƃ������𖼏���Ă��邽����2D�Փ˔���
    static bool IntersectAABBVsAABB(
        AABB* aabb1, AABB* aabb2,
        DirectX::XMFLOAT2& resultPos
    );

};

