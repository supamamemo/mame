#include "Collision.h"

// ���Ƌ��̌�������
bool Collision::IntersectSphereVsSphere(
    const DirectX::XMFLOAT3& positionA, const float radiusA,
    const DirectX::XMFLOAT3& positionB, const float radiusB,
    DirectX::XMFLOAT3& outPositionB)
{
    // A��B�̒P�ʃx�N�g�����Z�o
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
    float lengthSq = 0.0f;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);


    // ��������
    float range = radiusA + radiusB;    // A��B�̔��a�𑫂�������

    if (lengthSq > (range * range)) return false; // �������AB���a����������΂߂荞��ł��Ȃ�


    // A��B�������o��
    Vec       = DirectX::XMVector3Normalize(Vec); // ���K��
    Vec       = DirectX::XMVectorScale(Vec, range);
    PositionB = DirectX::XMVectorAdd(PositionA, Vec);
    DirectX::XMStoreFloat3(&outPositionB, PositionB);
 
    return true;
}


bool Collision::IntersectCylinderVsCylinder(
    const DirectX::XMFLOAT3& positionA, 
    const float& radiusA, 
    const float& heightA, 
    const DirectX::XMFLOAT3& positionB, 
    const float& radiusB, 
    const float& heightB, 
    DirectX::XMFLOAT3& outPositionB)
{
    // A�̑�����B�̓����ォ�AA�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
    if (positionA.y > positionB.y + heightB) return false;
    if (positionA.y + heightA < positionB.y) return false;
    

    // XZ���ʂł͈̔̓`�F�b�N
    float vx = positionB.x - positionA.x;
    float vz = positionB.z - positionA.z;
    const float range = radiusA + radiusB;
    const float dist = sqrtf(vx * vx + vz * vz);

    if (dist > range) return false;


    // A��B�������o��
    vx /= dist;
    vz /= dist;
    outPositionB.x = positionA.x + (vx * range);
    outPositionB.y = positionB.y; // ��A�ł͂Ȃ�B
    outPositionB.z = positionA.z + (vz * range);
    
    return true;
}


bool Collision::IntersectSphereVsCylinder(
    const DirectX::XMFLOAT3& spherePosition, 
    const float& sphereRadius, 
    const DirectX::XMFLOAT3& cylinderPosition, 
    const float& cylinderRadius, 
    const float& cylinderHeight, 
    DirectX::XMFLOAT3& outCylinderPosition)
{
    // �e�̑������~���̓����ォ�A�e�̓����~���̑�����艺�Ȃ瓖�����Ă��Ȃ�
    const float sphereDown   = spherePosition.y - sphereRadius;
    const float sphereUp     = spherePosition.y + sphereRadius;
    const float cylinderUp   = cylinderPosition.y + cylinderHeight;
    const float cylinderDown = cylinderPosition.y;

    if (sphereDown > cylinderUp) return false;
    if (sphereUp < cylinderDown) return false;


    // XZ���ʂł͈̔̓`�F�b�N
    float vx = cylinderPosition.x - spherePosition.x;
    float vz = cylinderPosition.z - spherePosition.z;
    const float range = sphereRadius + cylinderRadius;
    const float dist  = sqrtf(vx * vx + vz * vz);

    if (dist > range) return false;


    // A��B�������o��
    vx /= dist;
    vz /= dist;
    outCylinderPosition.x = cylinderPosition.x + (vx * range);
    outCylinderPosition.y = cylinderPosition.y;
    outCylinderPosition.z = cylinderPosition.z + (vz * range);

    return true;
}


//bool Collision::IntersectRayVsModel(
//    const DirectX::XMFLOAT3& start, 
//    const DirectX::XMFLOAT3& end, 
//    const Model* model, 
//    HitResult& result)
//{
//    const DirectX::XMVECTOR WorldStart     = DirectX::XMLoadFloat3(&start);
//    const DirectX::XMVECTOR WorldEnd       = DirectX::XMLoadFloat3(&end);
//    const DirectX::XMVECTOR WorldRayVec    = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
//    const DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);
//
//    // ���[���h��Ԃ̃��C�̒���
//    DirectX::XMStoreFloat(&result.distance, WorldRayLength);
//
//    NO_CONST bool hit = false;
//    const ModelResource* resource = model->GetResource();
//    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
//    {
//        // ���b�V���m�[�h�擾
//        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
//
//        // ���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϊ�
//        const DirectX::XMMATRIX WorldTransform        = DirectX::XMLoadFloat4x4(&node.worldTransform);
//        const DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);
//
//        const DirectX::XMVECTOR S  = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
//        const DirectX::XMVECTOR E  = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
//        const DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
//        const DirectX::XMVECTOR V  = DirectX::XMVector3Normalize(SE);
//        const DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);
//
//        // ���C�̒���
//        NO_CONST float neart =  DirectX::XMVectorGetX(Length);
//
//        // �O�p�`�i�ʁj�Ƃ̌�������
//        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
//        const std::vector<UINT> indices = mesh.indices;
//
//        NO_CONST int materialIndex = -1;
//        NO_CONST DirectX::XMVECTOR HitPosition = {};
//        NO_CONST DirectX::XMVECTOR HitNormal   = {};
//        for (const ModelResource::Subset& subset : mesh.subsets)
//        {
//            for (UINT i = 0; i < subset.indexCount; i += 3)
//            {
//                const UINT index = subset.startIndex + i;
//                
//                // �O�p�`�̒��_�𒊏o
//                const ModelResource::Vertex& a = vertices.at(indices.at(index));
//                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
//                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));
//
//                const DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
//                const DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
//                const DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);
//
//                // �O�p�`�̎O�Ӄx�N�g�����Z�o
//                const DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
//                const DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
//                const DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);
//
//                // �O�p�`�̖@���x�N�g�����Z�o
//                const DirectX::XMVECTOR N  = DirectX::XMVector3Cross(AB, BC);
//
//                // ���ς̌��ʂ��v���X�Ȃ�Η�����
//                const DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
//                const float dot = DirectX::XMVectorGetX(Dot);
//
//                // �������Ȃ�Փ˂��Ȃ�
//                if (dot >= 0.0f) continue; 
//
//
//                // ���C�ƕ��ʂ̌�_���Z�o
//                const DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
//                const DirectX::XMVECTOR X  = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), Dot);
//                const float x = DirectX::XMVectorGetX(X);
//
//                // ��_�܂ł̋��������܂łɌv�Z�����ŋߋ������傫���Ƃ��̓X�L�b�v
//                if (x < 0.0f || x > neart) continue;
//
//                // �O�p�`�̌�_P
//                const DirectX::XMVECTOR P = DirectX::XMVectorMultiplyAdd(V, X, S);
//
//                // ��_���O�p�`�̓����ɂ��邩����
//                {
//                    // 1��
//                    const DirectX::XMVECTOR PA     = DirectX::XMVectorSubtract(A, P);
//                    const DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
//                    const DirectX::XMVECTOR Dot1   = DirectX::XMVector3Dot(Cross1, N);
//                    const float dot1 = DirectX::XMVectorGetX(Dot1);
//                    if (dot1 < 0.0f) continue;
//
//                    // 2��
//                    const DirectX::XMVECTOR PB     = DirectX::XMVectorSubtract(B, P);
//                    const DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
//                    const DirectX::XMVECTOR Dot2   = DirectX::XMVector3Dot(Cross2, N);
//                    const float dot2 = DirectX::XMVectorGetX(Dot2);
//                    if (dot2 < 0.0f) continue;
//
//                    // 3��
//                    const DirectX::XMVECTOR PC     = DirectX::XMVectorSubtract(C, P);
//                    const DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
//                    const DirectX::XMVECTOR Dot3   = DirectX::XMVector3Dot(Cross3, N);
//                    const float dot3 = DirectX::XMVectorGetX(Dot3);
//                    if (dot3 < 0.0f) continue;
//                }
//
//                // �ŒZ������ۑ�
//                neart = x;
//
//                // ��_�Ɩ@�����X�V
//                HitPosition   = P;
//                HitNormal     = N;
//                materialIndex = subset.materialIndex;
//            }
//        }
//
//        if (materialIndex >= 0)
//        {
//            // ���[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
//            const DirectX::XMVECTOR WorldPosition    = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
//            const DirectX::XMVECTOR WorldCrossVec    = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
//            const DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
//            const float distance = DirectX::XMVectorGetX(WorldCrossLength);
//
//            // �q�b�g���ۑ�
//            if (result.distance > distance)
//            {
//                const DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);
//
//                result.distance      = distance;
//                result.materialIndex = materialIndex;
//                DirectX::XMStoreFloat3(&result.position, WorldPosition);
//                DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
//                hit = true;
//            }
//        }
//    }
//
//    return hit;
//}


const OBB Collision::CreateOBB(
    const DirectX::XMFLOAT3& center,
    const DirectX::XMFLOAT3& right,
    const DirectX::XMFLOAT3& up,
    const DirectX::XMFLOAT3& front,
    const DirectX::XMFLOAT3& scale)
{
    NO_CONST OBB obb = {};
    obb.center = center;
    obb.direction[0] = right;   // X
    obb.direction[1] = up;      // Y
    obb.direction[2] = front;   // Z
    obb.scale = scale;

    return obb;
}

// �������ɓ��e���ꂽ���������瓊�e���������Z�o
const float LenSegOnSeparateAxis(
    const DirectX::XMVECTOR& Sep, 
    const DirectX::XMVECTOR& e1, 
    const DirectX::XMVECTOR& e2,  
    const DirectX::XMVECTOR* e3 = nullptr)
{
    // 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
    // ������Sep�͕W��������Ă��邱��
    const XMVECTOR R1 = XMVectorAbs(XMVector3Dot(Sep, e1));
    const XMVECTOR R2 = XMVectorAbs(XMVector3Dot(Sep, e2));
    const XMVECTOR R3 = e3 ? XMVectorAbs(XMVector3Dot(Sep, *e3)) : XMVectorSet(0,0,0,0);

    NO_CONST float r1 = 0.0f;
    NO_CONST float r2 = 0.0f;
    NO_CONST float r3 = 0.0f;
    XMStoreFloat(&r1, R1);
    XMStoreFloat(&r2, R2);
    XMStoreFloat(&r3, R3);

    return r1 + r2 + r3;
}

bool Collision::IntersectOBBVsOBB(
    const OBB& obb1, const OBB& obb2,
    XMFLOAT3& correction)
{
    correction = {};

    const XMVECTOR Obb1Center = XMLoadFloat3(&obb1.center);
    const XMVECTOR Obb2Center = XMLoadFloat3(&obb2.center);
    const XMVECTOR Obb1Scale  = XMLoadFloat3(&obb1.scale);
    const XMVECTOR Obb2Scale  = XMLoadFloat3(&obb2.scale);

#if 0
    // OBB1�̃��[�J�����W�n���쐬
    const DirectX::XMVECTOR Obb1Center = DirectX::XMLoadFloat3(&obb1.center);
    const DirectX::XMVECTOR Obb1Rotation = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&obb1.rotation));
    const DirectX::XMVECTOR Obb1Scale = DirectX::XMLoadFloat3(&obb1.scale);
    const DirectX::XMMATRIX Obb1Transform = DirectX::XMMatrixAffineTransformation(
        Obb1Scale,
        DirectX::XMQuaternionIdentity(),
        Obb1Rotation,
        Obb1Center
    );

    // OBB2�̃��[�J�����W�n���쐬
    const DirectX::XMVECTOR Obb2Center = DirectX::XMLoadFloat3(&obb2.center);
    const DirectX::XMVECTOR Obb2Rotation = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&obb2.rotation));
    const DirectX::XMVECTOR Obb2Scale = DirectX::XMLoadFloat3(&obb2.scale);
    const DirectX::XMMATRIX Obb2Transform = DirectX::XMMatrixAffineTransformation(
        Obb2Scale,
        DirectX::XMQuaternionIdentity(),
        Obb2Rotation,
        Obb2Center
    );

    // OBB���m�̒��S�Ԃ̃x�N�g��
    const DirectX::XMVECTOR CenterVec = DirectX::XMVectorSubtract(Obb2Center, Obb1Center);

    // �eOBB�̔��a�x�N�g��
    const DirectX::XMVECTOR Obb1Radius = DirectX::XMVectorScale(Obb1Scale, 0.5f);
    const DirectX::XMVECTOR Obb2Radius = DirectX::XMVectorScale(Obb2Scale, 0.5f);


    // �e�������ɂ����铊�e�̕����v�Z
    for (int i = 0; i < 3; ++i)
    {
        const DirectX::XMVECTOR Obb1Axis = Obb1Transform.r[i];
        const DirectX::XMVECTOR Obb2Axis = Obb2Transform.r[i];

        const DirectX::XMVECTOR ProjectionWidth1 = DirectX::XMVector3Dot(Obb1Radius, Obb1Axis);
        const DirectX::XMVECTOR ProjectionWidth2 = DirectX::XMVector3Dot(Obb2Radius, Obb2Axis);
        const DirectX::XMVECTOR ProjectionWidth = DirectX::XMVectorAdd(Obb1Radius, Obb2Radius);

        // OBB���m�̓��e�����d�Ȃ��Ă��Ȃ��ꍇ�͏Փ˂��Ă��Ȃ��Ɣ���
        // (��XMVectorAbs�F�����̐�Βl��Ԃ�(���_0����̋���))
        // (��XMVector3Greater�F������V1��V2���傫�����ǂ������肷��)
        NO_CONST DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(CenterVec, Obb2Axis);
        NO_CONST DirectX::XMVECTOR Abs = DirectX::XMVectorAbs(Dot);

        //if (DirectX::XMVector3Greater(Abs, ProjectionWidth))
        if (DirectX::XMVector3Greater(Abs, ProjectionWidth))
        {
            correction = { 0,0,0 };
            return false;
        }

        const DirectX::XMVECTOR ObbAxisCross = DirectX::XMVector3Cross(Obb1Axis, Obb2Axis);
        const DirectX::XMVECTOR ObbAxisDot = DirectX::XMVector3Dot(CenterVec, ObbAxisCross);
        Dot = DirectX::XMVector3Dot(Obb1Axis, ObbAxisCross);
        const DirectX::XMVECTOR ObbAxisWidth = DirectX::XMVector3Dot(Dot, Obb1Radius);
        Abs = DirectX::XMVectorAbs(ObbAxisDot);

        if (DirectX::XMVector3Greater(Abs, ObbAxisWidth))
        {
            correction = { 0,0,0 };
            return false;
        }

        // OBB���m�̓��e�����d�Ȃ��Ă���ꍇ�A
        // �Փ˂����������̍ŏ������߂��x�N�g�����v�Z
        const DirectX::XMVECTOR AxisLengthSq = DirectX::XMVector3LengthSq(ObbAxisCross);
        const float             axisLengthSq = DirectX::XMVectorGetX(AxisLengthSq);

        const DirectX::XMVECTOR AxisDirection = DirectX::XMVector3Normalize(ObbAxisCross);
        const DirectX::XMVECTOR DividedValue = DirectX::XMVectorDivide(ObbAxisDot, AxisLengthSq);
        const float             fixDistance = DirectX::XMVectorGetX(DividedValue);

        const    DirectX::XMVECTOR FixVec = DirectX::XMVectorScale(AxisDirection, fixDistance);
        NO_CONST DirectX::XMFLOAT3 fixVec = {};
        DirectX::XMStoreFloat3(&fixVec, FixVec);

        const DirectX::XMVECTOR FixLengthSq = DirectX::XMVector3LengthSq(FixVec);
        const float             fixLengthSq = DirectX::XMVectorGetX(FixLengthSq);
        const DirectX::XMVECTOR FixVector = DirectX::XMLoadFloat3(&correction);
        const DirectX::XMVECTOR FixVectorLengthSq = DirectX::XMVector3LengthSq(FixVector);
        const float             fixVectorLengthSq = DirectX::XMVectorGetX(FixVectorLengthSq);

        // �����߂��x�N�g���̍ŏ��l���X�V
        if (i == 0 || fixLengthSq < fixVectorLengthSq)
        {
            correction = fixVec;
        }
    }

    // ���ׂĂ̎������ɂ����ďd�Ȃ��Ă���ꍇ�͏Փ˂��Ă���Ɣ���
    return true;
#else
    // �e�����x�N�g���̊m��
    // �iN***:�W����(���K��)�����x�N�g���j
    const XMVECTOR NAe1 = XMLoadFloat3(&obb1.direction[0]), Ae1 = NAe1 * Obb1Scale;
    const XMVECTOR NAe2 = XMLoadFloat3(&obb1.direction[0]), Ae2 = NAe2 * Obb1Scale;
    const XMVECTOR NAe3 = XMLoadFloat3(&obb1.direction[0]), Ae3 = NAe3 * Obb1Scale;
    const XMVECTOR NBe1 = XMLoadFloat3(&obb2.direction[0]), Be1 = NBe1 * Obb2Scale;
    const XMVECTOR NBe2 = XMLoadFloat3(&obb2.direction[0]), Be2 = NBe2 * Obb2Scale;
    const XMVECTOR NBe3 = XMLoadFloat3(&obb2.direction[0]), Be3 = NBe3 * Obb2Scale;
    const XMVECTOR Vec  = XMVectorSubtract(Obb2Center, Obb1Center);

    NO_CONST float rA = 0.0f;
    NO_CONST float rB = 0.0f;
    NO_CONST float L  = 0.0f;

    {
        // ������ : Ae1
        XMStoreFloat(&rA, XMVector3Length(Ae1));
        rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, &Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, NAe1)));
        if (L > rA + rB) return false; // �Փ˂��Ă��Ȃ�

        // ������ : Ae2
        XMStoreFloat(&rA, XMVector3Length(Ae2));
        rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, &Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, NAe2)));
        if (L > rA + rB) return false;

        // ������ : Ae3
        XMStoreFloat(&rA, XMVector3Length(Ae3));
        rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, &Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, NAe3)));
        if (L > rA + rB) return false;
    }
    {
        // ������ : Be1
        rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, &Ae3);
        XMStoreFloat(&rB, XMVector3Length(Be1));
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, NBe1)));
        if (L > rA + rB) return false;

        // ������ : Be2
        rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, &Ae3);
        XMStoreFloat(&rB, XMVector3Length(Be2));
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, NBe2)));
        if (L > rA + rB) return false;

        // ������ : Be3
        rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, &Ae3);
        XMStoreFloat(&rB, XMVector3Length(Be3));
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, NBe3)));
        if (L > rA + rB) return false;
    }

    NO_CONST XMVECTOR Cross = {};

    {
        // ������ : C11
        Cross = XMVector3Cross(NAe1, NBe1);
        rA    = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
        rB    = LenSegOnSeparateAxis(Cross, Be2, Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;

        // ������ : C12
        Cross = XMVector3Cross(NAe1, NBe2);
        rA    = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
        rB    = LenSegOnSeparateAxis(Cross, Be1, Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;

        // ������ : C13
        Cross = XMVector3Cross(NAe1, NBe3);
        rA    = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
        rB    = LenSegOnSeparateAxis(Cross, Be1, Be2);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;
    }

    {
        // ������ : C21
        Cross = XMVector3Cross(NAe2, NBe1);
        rA    = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
        rB    = LenSegOnSeparateAxis(Cross, Be2, Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;

        // ������ : C22
        Cross = XMVector3Cross(NAe2, NBe2);
        rA    = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
        rB    = LenSegOnSeparateAxis(Cross, Be1, Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;

        // ������ : C23
        Cross = XMVector3Cross(NAe2, NBe3);
        rA    = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
        rB    = LenSegOnSeparateAxis(Cross, Be1, Be2);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;
    }

    {
        // ������ : C31
        Cross = XMVector3Cross(NAe3, NBe1);
        rA    = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
        rB    = LenSegOnSeparateAxis(Cross, Be2, Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;

        // ������ : C32
        Cross = XMVector3Cross(NAe3, NBe2);
        rA    = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
        rB    = LenSegOnSeparateAxis(Cross, Be1, Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;

        // ������ : C33
        Cross = XMVector3Cross(NAe3, NBe3);
        rA    = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
        rB    = LenSegOnSeparateAxis(Cross, Be1, Be2);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;
    }

    // �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
    return true;

#endif
}

void Collision::FixOBB(
    OBB& obb1, OBB& obb2, 
    const DirectX::XMFLOAT3& fixVector)
{
    // �C���l���C���x�N�g���̔����ɐݒ�
    const DirectX::XMFLOAT3 correction = {
        fixVector.x * 0.5f,
        fixVector.y * 0.5f,
        fixVector.z * 0.5f,
    };

    // OBB1���C��
    obb1.center.x -= correction.x;
    obb1.center.y -= correction.y;
    obb1.center.z -= correction.z;    
    
    // OBB2���C��
    obb2.center.x += correction.x;
    obb2.center.y += correction.y;
    obb2.center.z += correction.z;
    
}