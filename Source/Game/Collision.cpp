#include "Collision.h"

#define ADJUST      (0.125f)

// 球と球の交差判定
bool Collision::IntersectSphereVsSphere(
    const DirectX::XMFLOAT3& positionA, const float radiusA,
    const DirectX::XMFLOAT3& positionB, const float radiusB,
    DirectX::XMFLOAT3& outPositionB)
{
    // A→Bの単位ベクトルを算出
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
    float lengthSq = 0.0f;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);


    // 距離判定
    float range = radiusA + radiusB;    // AとBの半径を足したもの

    if (lengthSq > (range * range)) return false; // 距離よりAB半径が小さければめり込んでいない


    // AがBを押し出す
    Vec       = DirectX::XMVector3Normalize(Vec); // 正規化
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
    // Aの足元がBの頭より上か、Aの頭がBの足元より下なら当たっていない
    if (positionA.y > positionB.y + heightB) return false;
    if (positionA.y + heightA < positionB.y) return false;
    

    // XZ平面での範囲チェック
    float vx = positionB.x - positionA.x;
    float vz = positionB.z - positionA.z;
    const float range = radiusA + radiusB;
    const float dist = sqrtf(vx * vx + vz * vz);

    if (dist > range) return false;


    // AがBを押し出す
    vx /= dist;
    vz /= dist;
    outPositionB.x = positionA.x + (vx * range);
    outPositionB.y = positionB.y; // ※AではなくB
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
    // 弾の足元が円柱の頭より上か、弾の頭が円柱の足元より下なら当たっていない
    const float sphereDown   = spherePosition.y - sphereRadius;
    const float sphereUp     = spherePosition.y + sphereRadius;
    const float cylinderUp   = cylinderPosition.y + cylinderHeight;
    const float cylinderDown = cylinderPosition.y;

    if (sphereDown > cylinderUp) return false;
    if (sphereUp < cylinderDown) return false;


    // XZ平面での範囲チェック
    float vx = cylinderPosition.x - spherePosition.x;
    float vz = cylinderPosition.z - spherePosition.z;
    const float range = sphereRadius + cylinderRadius;
    const float dist  = sqrtf(vx * vx + vz * vz);

    if (dist > range) return false;


    // AがBを押し出す
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
//    // ワールド空間のレイの長さ
//    DirectX::XMStoreFloat(&result.distance, WorldRayLength);
//
//    NO_CONST bool hit = false;
//    const ModelResource* resource = model->GetResource();
//    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
//    {
//        // メッシュノード取得
//        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
//
//        // レイをワールド空間からローカル空間へ変換
//        const DirectX::XMMATRIX WorldTransform        = DirectX::XMLoadFloat4x4(&node.worldTransform);
//        const DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);
//
//        const DirectX::XMVECTOR S  = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
//        const DirectX::XMVECTOR E  = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
//        const DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
//        const DirectX::XMVECTOR V  = DirectX::XMVector3Normalize(SE);
//        const DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);
//
//        // レイの長さ
//        NO_CONST float neart =  DirectX::XMVectorGetX(Length);
//
//        // 三角形（面）との交差判定
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
//                // 三角形の頂点を抽出
//                const ModelResource::Vertex& a = vertices.at(indices.at(index));
//                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
//                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));
//
//                const DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
//                const DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
//                const DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);
//
//                // 三角形の三辺ベクトルを算出
//                const DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
//                const DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
//                const DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);
//
//                // 三角形の法線ベクトルを算出
//                const DirectX::XMVECTOR N  = DirectX::XMVector3Cross(AB, BC);
//
//                // 内積の結果がプラスならば裏向き
//                const DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
//                const float dot = DirectX::XMVectorGetX(Dot);
//
//                // 裏向きなら衝突しない
//                if (dot >= 0.0f) continue; 
//
//
//                // レイと平面の交点を算出
//                const DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
//                const DirectX::XMVECTOR X  = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), Dot);
//                const float x = DirectX::XMVectorGetX(X);
//
//                // 交点までの距離が今までに計算した最近距離より大きいときはスキップ
//                if (x < 0.0f || x > neart) continue;
//
//                // 三角形の交点P
//                const DirectX::XMVECTOR P = DirectX::XMVectorMultiplyAdd(V, X, S);
//
//                // 交点が三角形の内側にあるか判定
//                {
//                    // 1つ目
//                    const DirectX::XMVECTOR PA     = DirectX::XMVectorSubtract(A, P);
//                    const DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
//                    const DirectX::XMVECTOR Dot1   = DirectX::XMVector3Dot(Cross1, N);
//                    const float dot1 = DirectX::XMVectorGetX(Dot1);
//                    if (dot1 < 0.0f) continue;
//
//                    // 2つ目
//                    const DirectX::XMVECTOR PB     = DirectX::XMVectorSubtract(B, P);
//                    const DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
//                    const DirectX::XMVECTOR Dot2   = DirectX::XMVector3Dot(Cross2, N);
//                    const float dot2 = DirectX::XMVectorGetX(Dot2);
//                    if (dot2 < 0.0f) continue;
//
//                    // 3つ目
//                    const DirectX::XMVECTOR PC     = DirectX::XMVectorSubtract(C, P);
//                    const DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
//                    const DirectX::XMVECTOR Dot3   = DirectX::XMVector3Dot(Cross3, N);
//                    const float dot3 = DirectX::XMVectorGetX(Dot3);
//                    if (dot3 < 0.0f) continue;
//                }
//
//                // 最短距離を保存
//                neart = x;
//
//                // 交点と法線を更新
//                HitPosition   = P;
//                HitNormal     = N;
//                materialIndex = subset.materialIndex;
//            }
//        }
//
//        if (materialIndex >= 0)
//        {
//            // ローカル空間からワールド空間へ変換
//            const DirectX::XMVECTOR WorldPosition    = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
//            const DirectX::XMVECTOR WorldCrossVec    = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
//            const DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
//            const float distance = DirectX::XMVectorGetX(WorldCrossLength);
//
//            // ヒット情報保存
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

// 分離軸に投影された軸成分から投影線分長を算出
const float LenSegOnSeparateAxis(
    const DirectX::XMVECTOR& Sep, 
    const DirectX::XMVECTOR& e1, 
    const DirectX::XMVECTOR& e2,  
    const DirectX::XMVECTOR* e3 = nullptr)
{
    // 3つの内積の絶対値の和で投影線分長を計算
    // 分離軸Sepは標準化されていること
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
    // OBB1のローカル座標系を作成
    const DirectX::XMVECTOR Obb1Center = DirectX::XMLoadFloat3(&obb1.center);
    const DirectX::XMVECTOR Obb1Rotation = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&obb1.rotation));
    const DirectX::XMVECTOR Obb1Scale = DirectX::XMLoadFloat3(&obb1.scale);
    const DirectX::XMMATRIX Obb1Transform = DirectX::XMMatrixAffineTransformation(
        Obb1Scale,
        DirectX::XMQuaternionIdentity(),
        Obb1Rotation,
        Obb1Center
    );

    // OBB2のローカル座標系を作成
    const DirectX::XMVECTOR Obb2Center = DirectX::XMLoadFloat3(&obb2.center);
    const DirectX::XMVECTOR Obb2Rotation = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&obb2.rotation));
    const DirectX::XMVECTOR Obb2Scale = DirectX::XMLoadFloat3(&obb2.scale);
    const DirectX::XMMATRIX Obb2Transform = DirectX::XMMatrixAffineTransformation(
        Obb2Scale,
        DirectX::XMQuaternionIdentity(),
        Obb2Rotation,
        Obb2Center
    );

    // OBB同士の中心間のベクトル
    const DirectX::XMVECTOR CenterVec = DirectX::XMVectorSubtract(Obb2Center, Obb1Center);

    // 各OBBの半径ベクトル
    const DirectX::XMVECTOR Obb1Radius = DirectX::XMVectorScale(Obb1Scale, 0.5f);
    const DirectX::XMVECTOR Obb2Radius = DirectX::XMVectorScale(Obb2Scale, 0.5f);


    // 各軸方向における投影の幅を計算
    for (int i = 0; i < 3; ++i)
    {
        const DirectX::XMVECTOR Obb1Axis = Obb1Transform.r[i];
        const DirectX::XMVECTOR Obb2Axis = Obb2Transform.r[i];

        const DirectX::XMVECTOR ProjectionWidth1 = DirectX::XMVector3Dot(Obb1Radius, Obb1Axis);
        const DirectX::XMVECTOR ProjectionWidth2 = DirectX::XMVector3Dot(Obb2Radius, Obb2Axis);
        const DirectX::XMVECTOR ProjectionWidth = DirectX::XMVectorAdd(Obb1Radius, Obb2Radius);

        // OBB同士の投影幅が重なっていない場合は衝突していないと判定
        // (※XMVectorAbs：引数の絶対値を返す(原点0からの距離))
        // (※XMVector3Greater：引数のV1がV2より大きいかどうか判定する)
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

        // OBB同士の投影幅が重なっている場合、
        // 衝突した軸方向の最小押し戻しベクトルを計算
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

        // 押し戻しベクトルの最小値を更新
        if (i == 0 || fixLengthSq < fixVectorLengthSq)
        {
            correction = fixVec;
        }
    }

    // すべての軸方向において重なっている場合は衝突していると判定
    return true;
#else
    // 各方向ベクトルの確保
    // （N***:標準化(正規化)方向ベクトル）
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
        // 分離軸 : Ae1
        XMStoreFloat(&rA, XMVector3Length(Ae1));
        rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, &Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, NAe1)));
        if (L > rA + rB) return false; // 衝突していない

        // 分離軸 : Ae2
        XMStoreFloat(&rA, XMVector3Length(Ae2));
        rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, &Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, NAe2)));
        if (L > rA + rB) return false;

        // 分離軸 : Ae3
        XMStoreFloat(&rA, XMVector3Length(Ae3));
        rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, &Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, NAe3)));
        if (L > rA + rB) return false;
    }
    {
        // 分離軸 : Be1
        rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, &Ae3);
        XMStoreFloat(&rB, XMVector3Length(Be1));
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, NBe1)));
        if (L > rA + rB) return false;

        // 分離軸 : Be2
        rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, &Ae3);
        XMStoreFloat(&rB, XMVector3Length(Be2));
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, NBe2)));
        if (L > rA + rB) return false;

        // 分離軸 : Be3
        rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, &Ae3);
        XMStoreFloat(&rB, XMVector3Length(Be3));
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, NBe3)));
        if (L > rA + rB) return false;
    }

    NO_CONST XMVECTOR Cross = {};

    {
        // 分離軸 : C11
        Cross = XMVector3Cross(NAe1, NBe1);
        rA    = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
        rB    = LenSegOnSeparateAxis(Cross, Be2, Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;

        // 分離軸 : C12
        Cross = XMVector3Cross(NAe1, NBe2);
        rA    = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
        rB    = LenSegOnSeparateAxis(Cross, Be1, Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;

        // 分離軸 : C13
        Cross = XMVector3Cross(NAe1, NBe3);
        rA    = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
        rB    = LenSegOnSeparateAxis(Cross, Be1, Be2);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;
    }

    {
        // 分離軸 : C21
        Cross = XMVector3Cross(NAe2, NBe1);
        rA    = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
        rB    = LenSegOnSeparateAxis(Cross, Be2, Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;

        // 分離軸 : C22
        Cross = XMVector3Cross(NAe2, NBe2);
        rA    = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
        rB    = LenSegOnSeparateAxis(Cross, Be1, Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;

        // 分離軸 : C23
        Cross = XMVector3Cross(NAe2, NBe3);
        rA    = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
        rB    = LenSegOnSeparateAxis(Cross, Be1, Be2);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;
    }

    {
        // 分離軸 : C31
        Cross = XMVector3Cross(NAe3, NBe1);
        rA    = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
        rB    = LenSegOnSeparateAxis(Cross, Be2, Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;

        // 分離軸 : C32
        Cross = XMVector3Cross(NAe3, NBe2);
        rA    = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
        rB    = LenSegOnSeparateAxis(Cross, Be1, Be3);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;

        // 分離軸 : C33
        Cross = XMVector3Cross(NAe3, NBe3);
        rA    = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
        rB    = LenSegOnSeparateAxis(Cross, Be1, Be2);
        XMStoreFloat(&L, XMVectorAbs(XMVector3Dot(Vec, Cross)));
        if (L > rA + rB) return false;
    }

    // 分離平面が存在しないので「衝突している」
    return true;

#endif
}

void Collision::FixOBB(
    OBB& obb1, OBB& obb2, 
    const DirectX::XMFLOAT3& fixVector)
{
    // 修正値を修正ベクトルの半分に設定
    const DirectX::XMFLOAT3 correction = {
        fixVector.x * 0.5f,
        fixVector.y * 0.5f,
        fixVector.z * 0.5f,
    };

    // OBB1を修正
    obb1.center.x -= correction.x;
    obb1.center.y -= correction.y;
    obb1.center.z -= correction.z;    
    
    // OBB2を修正
    obb2.center.x += correction.x;
    obb2.center.y += correction.y;
    obb2.center.z += correction.z;
    
}





bool Collision::IntersectAABBVsAABB(
    AABB* aabb1, AABB* aabb2,
    DirectX::XMFLOAT3& resultPos
)
{
    float aabb1Right = aabb1->center.x + aabb1->range.x;
    float aabb1Left = aabb1->center.x - aabb1->range.x;
    float aabb1Top = aabb1->center.y + aabb1->range.y;
    float aabb1Bottom = aabb1->center.y - aabb1->range.y;
    float aabb2Right = aabb2->center.x + aabb2->range.x;
    float aabb2Left = aabb2->center.x - aabb2->range.x;
    float aabb2Top = aabb2->center.y + aabb2->range.y;
    float aabb2Bottom = aabb2->center.y - aabb2->range.y;

    // 当たっていない
    if (aabb1Right < aabb2Left)return false;
    if (aabb1Left > aabb2Right)return false;
    if (aabb1Top < aabb2Bottom)return false;
    if (aabb1Bottom > aabb2Top)return false;

    
    if (aabb1Right > aabb2Left || aabb1Left < aabb2Right)
    {
        if (aabb1Right > aabb2Right)
            resultPos.x = aabb2Left - aabb1Right - ADJUST;    // checkRight
        if (aabb1Left < aabb2Left)
            resultPos.x = aabb2Right - aabb1Left - ADJUST;    // checkLeft
    }

    if (aabb1Top > aabb2Bottom || aabb1Bottom < aabb2Top)
    {
        if (aabb1Top > aabb2Bottom)
            resultPos.y = aabb2Bottom - aabb1Top - ADJUST;    // checkTop
        if (aabb1Bottom < aabb2Bottom)
            resultPos.y = aabb2Top - aabb1Bottom - ADJUST;    // checkDown
    }

    return true;
}