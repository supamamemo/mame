#pragma once

#include <DirectXMath.h>
#include "../Mame/Graphics/Model.h"
#include "Common.h"


// コリジョン
class Collision
{
public:
#if 0
    // ヒット結果
    struct HitResult
    {
        DirectX::XMFLOAT3 position = { 0,0,0 }; // レイとポリゴンの交点
        DirectX::XMFLOAT3 normal = { 0,0,0 }; // 衝突したポリゴンの法線ベクトル
        float             distance = 0.0f;      // レイの始点から交点までの距離
        int               materialIndex = -1;        // 衝突したポリゴンのマテリアル番号
        DirectX::XMFLOAT3 rotation = { 0,0,0 }; // 回転量
    };

    //// 今回はZ軸がないからXMFLOAT2。何ならこれはAABBではない（笑）
    //struct AABB
    //{
    //    DirectX::XMFLOAT3 center;   // 中心点の座標
    //    DirectX::XMFLOAT3 range;    // 半径
    //};
#endif

    struct Box3D
    {
        DirectX::XMFLOAT3 position = {};
        //DirectX::XMFLOAT3 lenght    = {};
        DirectX::XMFLOAT3 min = {};
        DirectX::XMFLOAT3 max = {};

        // lenghtは半径的な奴。
        // キューブの長さの半分

        // ーーーーー*ーーーーー
        // |                 |
        // |                 |
        // |                 |
        // |--------*        |
        // | l e n  | l      |
        // |        | e      |
        // |        | n      |
        // ーーーーー*ーーーーー
        //      position
    };

public:
#if 0
    // 球と球の交差判定
    static bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA, const float& radiusA,
        const DirectX::XMFLOAT3& positionB, const float& radiusB,
        DirectX::XMFLOAT3& outPositionB
    );

    // 円柱と円柱の交差判定
    static bool IntersectCylinderVsCylinder(
        const DirectX::XMFLOAT3& positionA,
        const float& radiusA, 
        const float& heightA,
        const DirectX::XMFLOAT3& positionB,
        const float& radiusB, 
        const float& heightB,
        DirectX::XMFLOAT3& outPositionB
    );

    // 球と円柱の交差判定
    static bool IntersectSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition,
        const float& sphereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        const float& cylinderRadius,
        const float& cylinderHeight,
        DirectX::XMFLOAT3& outCylinderPosition
    );

    // レイとモデルの交差判定
    static bool IntersectRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );

    //// AABBという名を名乗っているただの2D衝突判定
    //static bool IntersectAABBVsAABB(
    //    AABB* aabb1, AABB* aabb2,
    //    DirectX::XMFLOAT3& resultPos
    //);
#endif

    // Box3D同士の交差判定
    static bool IntersectBox3DVsBox3D(
        const    Box3D& box1, 
        const    Box3D& box2, 
        NO_CONST Box3D& outBoxPosition
    );

};

