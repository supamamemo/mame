#pragma once

#include <DirectXMath.h>
using namespace DirectX;

//#include "Graphics/Graphics.h"
#include "../Mame/Graphics/Model.h"
#include "Common.h"

// ヒット結果
struct HitResult
{
    DirectX::XMFLOAT3 position  = { 0,0,0 }; // レイとポリゴンの交点
    DirectX::XMFLOAT3 normal    = { 0,0,0 }; // 衝突したポリゴンの法線ベクトル
    float distance      = 0.0f;              // レイの始点から交点までの距離
    int   materialIndex = -1;                // 衝突したポリゴンのマテリアル番号
    DirectX::XMFLOAT3 rotation  = { 0,0,0 }; // 回転量
};

struct OBB
{
    DirectX::XMFLOAT3  center       = {}; // 中心点の座標
    DirectX::XMFLOAT3  direction[3] = {}; // XYZの各方向ベクトル(正規化されていること)
    DirectX::XMFLOAT3  scale        = {}; // スケール
};

// 今回はZ軸がないからXMFLOAT2。何ならこれはAABBではない（笑）
struct AABB
{
    DirectX::XMFLOAT2 center;   // 中心点の座標
    DirectX::XMFLOAT2 range;    // 半径
};

// コリジョン
class Collision
{
public:
    // 球と球の交差判定
    static bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA, const float radiusA,
        const DirectX::XMFLOAT3& positionB, const float radiusB,
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

    // OBBの作成
    // center：中心点の座標
    // direction：XYZの各方向ベクトル
    // scale：OBBの各座標軸に沿った長さの半分(中心点から面までの長さ)
    static const OBB CreateOBB(
        const DirectX::XMFLOAT3& center,
        const DirectX::XMFLOAT3& right,
        const DirectX::XMFLOAT3& up,
        const DirectX::XMFLOAT3& front,
        const DirectX::XMFLOAT3& scale
    );

    // OBBとOBBの衝突判定
    // fixVector：修正値
    static bool IntersectOBBVsOBB(
        const OBB& obb1, const OBB& obb2,
        DirectX::XMFLOAT3& correction
    );

    // 衝突後のOBB修正
    // ※修正されたOBBの各パラメータを元のパラメーターに代入すること
    static void FixOBB(
        OBB& obb1, OBB& obb2,
        const DirectX::XMFLOAT3& fixVector
    );


    // AABBという名を名乗っているただの2D衝突判定
    static bool IntersectAABBVsAABB(
        AABB* aabb1, AABB* aabb2,
        DirectX::XMFLOAT2& resultPos
    );

};

