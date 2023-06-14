#pragma once

#include <DirectXMath.h>
using namespace DirectX;

//#include "Graphics/Graphics.h"
#include "../Mame/Graphics/Model.h"
#include "Common.h"

// �q�b�g����
struct HitResult
{
    DirectX::XMFLOAT3 position  = { 0,0,0 }; // ���C�ƃ|���S���̌�_
    DirectX::XMFLOAT3 normal    = { 0,0,0 }; // �Փ˂����|���S���̖@���x�N�g��
    float distance      = 0.0f;              // ���C�̎n�_�����_�܂ł̋���
    int   materialIndex = -1;                // �Փ˂����|���S���̃}�e���A���ԍ�
    DirectX::XMFLOAT3 rotation  = { 0,0,0 }; // ��]��
};

struct OBB
{
    DirectX::XMFLOAT3  center       = {}; // ���S�_�̍��W
    DirectX::XMFLOAT3  direction[3] = {}; // XYZ�̊e�����x�N�g��(���K������Ă��邱��)
    DirectX::XMFLOAT3  scale        = {}; // �X�P�[��
};

// �R���W����
class Collision
{
public:
    // ���Ƌ��̌�������
    static bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA, const float radiusA,
        const DirectX::XMFLOAT3& positionB, const float radiusB,
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

    // OBB�̍쐬
    // center�F���S�_�̍��W
    // direction�FXYZ�̊e�����x�N�g��
    // scale�FOBB�̊e���W���ɉ����������̔���(���S�_����ʂ܂ł̒���)
    static const OBB CreateOBB(
        const DirectX::XMFLOAT3& center,
        const DirectX::XMFLOAT3& right,
        const DirectX::XMFLOAT3& up,
        const DirectX::XMFLOAT3& front,
        const DirectX::XMFLOAT3& scale
    );

    // OBB��OBB�̏Փ˔���
    // fixVector�F�C���l
    static bool IntersectOBBVsOBB(
        const OBB& obb1, const OBB& obb2,
        DirectX::XMFLOAT3& correction
    );

    // �Փˌ��OBB�C��
    // ���C�����ꂽOBB�̊e�p�����[�^�����̃p�����[�^�[�ɑ�����邱��
    static void FixOBB(
        OBB& obb1, OBB& obb2,
        const DirectX::XMFLOAT3& fixVector
    );
};
