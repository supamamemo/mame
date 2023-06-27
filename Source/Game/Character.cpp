#include "Character.h"

#include "../Mame/Graphics/Graphics.h"
#include "../misc.h"

#include "OperatorXMFLOAT3.h"
#include "Mathf.h"
#include "Collision.h"
#include "Terrain/TerrainManager.h"

Character::Character()
{
    Graphics& graphics = Graphics::Instance();

    create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/wireframe.cso", pixel_shaders.GetAddressOf());

    geometricAABB_ = std::make_unique<GeometricAABB>(graphics.GetDevice(), defaultMin_, defaultMax_);
}


void Character::Render(const float& /*elapsedTime*/)
{
    Graphics& graphics = Graphics::Instance();

    // world行列更新
    NO_CONST DirectX::XMFLOAT4X4 transform = {};
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model描画
    if (&model->keyframe)
    {
        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, DirectX::XMFLOAT4(1, 1, 1, 1), &model->keyframe);
    }
    else
    {
        model->skinned_meshes.render(graphics.GetDeviceContext(), transform, DirectX::XMFLOAT4(1, 1, 1, 1), nullptr);
    }


#if _DEBUG

    // デバッグモデルでの描画
#if 0
    DirectX::XMFLOAT4X4 debugTransform = {};

    // ワールド行列の取得とスケール調整
    DirectX::XMStoreFloat4x4(&debugTransform, debugModel->GetTransform()->CalcWorldMatrix(0.01f));

    // ワールド行列設定
    debugTransform = SetDebugModelTransform(debugTransform);

    // 描画
    debugModel->skinned_meshes.render(graphics.GetDeviceContext(), debugTransform, { 1.0f, 0.0f, 0.0f, 0.2f }, nullptr);
#endif

    // ラスタライザステート作成・設定
#if 0
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state = nullptr;
    
    // ラスタライザステート作成
    NO_CONST HRESULT hr = {};
    NO_CONST D3D11_RASTERIZER_DESC rasterizer_desc = {};
    //rasterizer_desc.FillMode                = D3D11_FILL_WIREFRAME; // 塗りつぶし
    rasterizer_desc.FillMode                = D3D11_FILL_SOLID; // 塗りつぶし
    //rasterizer_desc.CullMode                = D3D11_CULL_BACK;	    // 背面カリング（裏面を描画しない）
    rasterizer_desc.CullMode                = D3D11_CULL_NONE;	    // カリングなし

    // ?
    //rasterizer_desc.FrontCounterClockwise   = FALSE;			    // 三角形をを反時計回り（裏側）にするか
    rasterizer_desc.FrontCounterClockwise   = TRUE;			    // 三角形をを反時計回り（裏側）にするか

    rasterizer_desc.DepthBias               = 0;	                // 指定されたピクセルに追加された深度値
    rasterizer_desc.DepthBiasClamp          = 0;	                // ピクセルの最大深度バイアス
    rasterizer_desc.SlopeScaledDepthBias    = 0;	                // 指定されたピクセルの傾きのスカラー
    rasterizer_desc.DepthClipEnable         = TRUE;	                // 距離によるクリッピングを行うかのフラグ
    rasterizer_desc.ScissorEnable           = FALSE;                // シザー矩形カリングを行うかのフラグ
    rasterizer_desc.MultisampleEnable       = FALSE;                // マルチサンプリングアンチエイリアス(略称：MSAA)のレンダーターゲットを使用している時、四辺形ラインアンチエイリアスを行うか、アルファラインアンチエイリアスを行うかを決めるフラグ
    rasterizer_desc.AntialiasedLineEnable   = FALSE;                // MSAAのレンダーターゲットを使用している時、線分描画でMultisampleEnableがfalseのとき、アンチエイリアスを有効にします
    hr = graphics.GetDevice()->CreateRasterizerState(&rasterizer_desc, rasterizer_state.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    // ラスタライザステート設定(ワイヤーフレーム)
    graphics.GetDeviceContext()->RSSetState(rasterizer_state.Get());

#else
    // RS番号
    {
        // 0 ソリッド・後ろカリング
        // 1 ワイヤーフレーム・後ろカリング
        // 2 ワイヤーフレーム・カリングなし
        // 3 ソリッド・カリングなし
    }

    // ラスタライザ設定(ソリッド・カリングなし)
    graphics.GetShader()->SetState(graphics.GetDeviceContext(), 3, 0, 0);

#endif
    // 回転なしワールド行列の作成
    NO_CONST  DirectX::XMFLOAT4X4 noRotationTransform = {};
    {
        const DirectX::XMFLOAT3  scale      = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
        const DirectX::XMFLOAT3& position   = GetTransform()->GetPosition();
        const DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
        const DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        DirectX::XMStoreFloat4x4(&noRotationTransform, S * T);
    }

    // カラー設定
    //const DirectX::XMFLOAT4 materialColor = { 1, 0, 0, 0.4f };
    
    // AABB描画
    geometricAABB_->render(graphics.GetDeviceContext(), noRotationTransform, materialColor);

    // ラスタライザ再設定(ソリッド・後ろカリング)
    graphics.GetShader()->SetState(graphics.GetDeviceContext(), 0, 0, 0);

#endif // _DEBUG
}


void Character::DrawDebug()
{
    // ImGui描画
    GetTransform()->DrawDebug();
}


// 移動処理
void Character::Move(
    const float& vx, 
    const float& moveSpeed)
{
    // 移動方向ベクトルを設定
    moveVecX = vx;

    // 最大速度設定
    maxMoveSpeed = moveSpeed; // elapsedTimeを乗算しない
}


// 旋回処理
void Character::Turn(
    const    float& elapsedTime,
    NO_CONST float vx,  
    NO_CONST float turnSpeed)
{
    // 進行ベクトルがゼロベクトルの場合は処理する必要なし
    if (vx == 0.0f) return;

    NO_CONST DirectX::XMFLOAT4 rotation = GetTransform()->GetRotation();

    turnSpeed *= elapsedTime;

    // 進行ベクトルを単位ベクトル化
    const float vLength = sqrtf(vx * vx);
    if (vLength < 0.001f) return;

    // 単位ベクトル化
    vx /= vLength;

    // 自身の回転値から前方向を求める
    const float frontX = sinf(rotation.y);
    const float frontZ = cosf(rotation.y);

    // 回転角を求めるため、2つの単位ベクトルの内積計算する
    const float dot = (frontX * vx) /*+ (frontZ * vz)*/;

    // 内積値は-1.0~1.0で表現されており、2つの単位ベクトルの角度が
    // 小さいほど1.0に近づくという性質を利用して回転速度を調整する  
    NO_CONST float rot = 1.0f - dot;   // 補正値                    
    if (rot > turnSpeed) rot = turnSpeed;

    // 左右判定を行うために2つの単位ベクトルの外積を計算する
    const float cross = (frontZ * vx) /*- (frontX * vz)*/;

    // 2Dの外積値が正の場合か負の場合によって左右判定が行える
    // 左右判定を行うことによって左右回転を選択する
    rotation.y += (cross < 0.0f) ? -rot : rot;
    
    GetTransform()->SetRotation(rotation);
}


// ジャンプ処理
void Character::Jump(const float& jumpSpeed)
{
    // 上方向の力を設定
    velocity.y = jumpSpeed;
}


// 速力更新処理
void Character::UpdateVelocity(const float& elapsedTime)
{
    // 経過フレーム（経過時間が変動した場合でも正常に計算されるようにする）
    const float elapsedFrame = 60.0f * elapsedTime;

    // 垂直速力更新処理
    UpdateVerticalVelocity(elapsedFrame);

    // 垂直移動更新処理
    UpdateVerticalMove(elapsedTime);

    // 水平速力更新処理
    UpdateHorizontalVelocity(elapsedFrame);

    // 水平移動更新処理
    UpdateHorizontalMove(elapsedTime);
}


// AABB更新処理
void Character::UpdateAABB()
{
    const DirectX::XMFLOAT3& position = GetTransform()->GetPosition();

    aabb_.min = position + defaultMin_;
    aabb_.max = position + defaultMax_;
}


// 無敵時間更新処理
void Character::UpdateInvincibleTimer(const float& elapsedTime)
{
    //　無敵時間がなければ飛ばす
    if (invincibleTimer <= 0.0f) return;

    // 無敵時間を減らす
    invincibleTimer -= elapsedTime;
}

// 垂直速力更新処理
void Character::UpdateVerticalVelocity(const float& elapsedFrame)
{
    // 重力処理
    velocity.y += gravity * elapsedFrame;
    const float gravityMax = -20.0f;
    if (velocity.y < gravityMax) velocity.y = gravityMax;
}

// 垂直移動更新処理
void Character::UpdateVerticalMove(const float& elapsedTime)
{
    NO_CONST Transform* transform = GetTransform();

    // 垂直方向の移動量
    const float my = velocity.y * elapsedTime;

    // 落下中
    if (my < 0.0f)
    {
        NO_CONST TerrainManager& terrainManager = TerrainManager::Instance();
        const int terrainCount = terrainManager.GetTerrainCount();

        NO_CONST bool isHit = false;

        for (int i = 0; i < terrainCount; ++i)
        {
            const Terrain* terrain = terrainManager.GetTerrain(i);

            NO_CONST DirectX::XMFLOAT3 pushVec = {};
            if (Collision::IntersectAABBVsAABB(aabb_, terrain->aabb_, pushVec))
            {

                isHit = true;


                velocity.y = 0.0f;

                // Y軸に重なっている値を求める（エネミーの頭からプレイヤーの足元までの距離）
                // 位置がマイナスでも問題ないように絶対値に変換しておく
                const float overlapUp       = fabsf( fabsf(aabb_.min.y) - fabsf(terrain->aabb_.max.y) ); // 上からめり込んでいる
                const float overlapBottom   = fabsf( fabsf(aabb_.max.y) - fabsf(terrain->aabb_.min.y) ); // 下からめり込んでいる
                //const float overlapRight    = fabsf( fabsf(aabb_.min.x) - fabsf(terrain->aabb_.max.x) ); // 右からめり込んでいる
                //const float overlapLeft     = fabsf( fabsf(aabb_.max.x) - fabsf(terrain->aabb_.min.x) ); // 左からめり込んでいる
                // 一番重なっていない値を求める
                const float overlapY        = (std::min)(overlapUp, overlapBottom);
                //const float overlapX        = (std::min)(overlapRight, overlapLeft);
                //const float overlap         = (std::min)(overlapY, overlapX);

                // 重なりが微小であれば修正しない(細かく修正しすぎると着地モーションが延々と続く)
                if (overlapY <= 0.001f) continue;

                // 重なっている分だけ押し戻す
                if (overlapY == overlapUp)
                {
                    // バウンス中は跳ねさせる
                    if (isBounce)
                    {
                        OnBounce();
                    }
                    else
                    {
                        GetTransform()->AddPositionY(overlapY);

                        // 着地した
                        if (!isGround) OnLanding();
                        isGround = true;
                    }
                }
                else if (overlapY == overlapBottom)
                {
                    GetTransform()->AddPositionY(-overlapY + -0.1f);
                }
                //else if (overlap == overlapRight)   GetTransform()->AddPositionX(overlap);
                //else if (overlap == overlapUp)      GetTransform()->AddPositionX(-overlap);

                
                // 押し戻し後のAABBの最小座標と最大座標を更新
                UpdateAABB();
                
                
            }
        }

        if (!isHit)
        {
            // 空中に浮いている
            transform->AddPositionY(my);
            isGround = false;
        }
    }
    // 上昇中
    else if (my > 0.0f)
    {
        isGround = false;

        transform->AddPositionY(my);
        UpdateAABB();

        NO_CONST TerrainManager& terrainManager = TerrainManager::Instance();
        const int terrainCount = terrainManager.GetTerrainCount();

        NO_CONST bool isHit = false;

        for (int i = 0; i < terrainCount; ++i)
        {
            const Terrain* terrain = terrainManager.GetTerrain(i);

            NO_CONST DirectX::XMFLOAT3 pushVec = {};
            if (Collision::IntersectAABBVsAABB(aabb_, terrain->aabb_, pushVec))
            {

                isHit = true;

                // Y軸に重なっている値を求める（エネミーの頭からプレイヤーの足元までの距離）
                // 位置がマイナスでも問題ないように絶対値に変換しておく
                //const float overlapUp     = fabsf(fabsf(aabb_.min.y) - fabsf(terrain->aabb_.max.y)); // 上からめり込んでいる
                const float overlapBottom = fabsf(fabsf(aabb_.max.y) - fabsf(terrain->aabb_.min.y)); // 下からめり込んでいる
                //const float overlapRight    = fabsf( fabsf(aabb_.min.x) - fabsf(terrain->aabb_.max.x) ); // 右からめり込んでいる
                //const float overlapLeft     = fabsf( fabsf(aabb_.max.x) - fabsf(terrain->aabb_.min.x) ); // 左からめり込んでいる
                // 一番重なっていない値を求める
                //const float overlapY = (std::min)(overlapUp, overlapBottom);
                //const float overlapX        = (std::min)(overlapRight, overlapLeft);
                //const float overlap         = (std::min)(overlapY, overlapX);

                // 重なりが微小であれば修正しない(細かく修正しすぎると着地モーションが延々と続く)
                //if (overlapY <= 0.001f) continue;
                if (overlapBottom <= 0.001f) continue;

                //// 重なっている分だけ押し戻す
                //if (overlapY == overlapUp)
                //{
                //    GetTransform()->AddPositionY(overlapY);
                //}
                //else if (overlapY == overlapBottom)
                //{
                    GetTransform()->AddPositionY(-overlapBottom + -0.1f);

                    velocity.y = -velocity.y;
                //}
                //else if (overlap == overlapRight)   GetTransform()->AddPositionX(overlap);
                //else if (overlap == overlapUp)      GetTransform()->AddPositionX(-overlap);


                // 押し戻し後のAABBの最小座標と最大座標を更新
                UpdateAABB();

            }
        }

        //if (!isHit)
        //{
        //    // 空中に浮いている
        //    transform->AddPositionY(my);
        //    isGround = false;
        //}

    }
}


// 水平速力更新処理
void Character::UpdateHorizontalVelocity(const float& elapsedFrame)
{
    const float dist = sqrtf(velocity.x * velocity.x);

    // XZ平面の速力を減速する
    if (dist > 0.0f)
    {
        // 摩擦力
        float friction = this->friction * elapsedFrame;
        // 空中にいるときは摩擦力を減らす
        if (!isGround) friction *= airControl;

        // 摩擦による横方向の減速処理
        if (dist > friction)
        {
            // 単位ベクトル化
            const float vx = velocity.x / dist;

            velocity.x -= vx * friction;
        }
        // 横方向の速力が摩擦力以下になったら速力を無効化
        else
        {
            velocity.x = 0.0f;
        }
    }

    // ダッシュしていたらダッシュ処理を行う
    if (isDash)
    {
        OnDash();
    }
    // X軸の速力を加速する
    else if (dist <= maxMoveSpeed)
    {
        // 移動ベクトルがゼロベクトルでないなら加速する
        const float moveVecDist = sqrtf(moveVecX * moveVecX);
        if (moveVecDist > 0.0f)
        {
            // 加速力
            float acceleration = this->acceleration * elapsedFrame;

            // 空中にいるときは加速力を減らす
            if (!isGround) acceleration *= airControl;

            // 移動ベクトルによる加速処理
            velocity.x += moveVecX * acceleration;

            // 最大速度制限
            const float dist = sqrtf(velocity.x * velocity.x);
            if (dist > maxMoveSpeed)
            {
                velocity.x = moveVecX * maxMoveSpeed;
            }

            // 下り坂でガタガタしないようにする
            if (isGround && slopeRate > 0.0f) // 着地時はスロープ対応
            {
                velocity.y -= dist * slopeRate * elapsedFrame;
            }
        }
    }

    // 移動ベクトルをリセット
    moveVecX = 0.0f;
}

// 水平移動更新処理
void Character::UpdateHorizontalMove(const float& elapsedTime)
{
    Transform* transform = GetTransform();

    // 水平速力量計算
    const float velocityLengthX = sqrtf(velocity.x * velocity.x);

    if (velocityLengthX > 0.0f)
    {
        // 水平移動値
        const float mx = velocity.x * elapsedTime;


        {
            // 移動処理
            transform->AddPositionX(mx);
        }
    }

}


void Character::ResetAABB(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
{
    // min・maxの再設定
    defaultMin_ = min;
    defaultMax_ = max;

    // 当たり判定のAABB描画を再設定
    ID3D11Device* device = Graphics::Instance().GetDevice();
    geometricAABB_ = std::make_unique<GeometricAABB>(device, defaultMin_, defaultMax_);
}
