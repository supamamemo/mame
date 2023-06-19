#include "Character.h"
#include "Collision.h"
#include "Mathf.h"

void Character::DrawDebug()
{
    // ImGui描画
    model->GetTransform()->DrawDebug();

    ImGui::Begin("debugmodel");
    debugModel->GetTransform()->DrawDebug();
    ImGui::End();
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

    NO_CONST DirectX::XMFLOAT4 rotation = model->GetTransform()->GetRotation();

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
    
    model->GetTransform()->SetRotation(rotation);
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
}

// 垂直移動更新処理
void Character::UpdateVerticalMove(const float& elapsedTime)
{
    NO_CONST DirectX::XMFLOAT3 position = model->GetTransform()->GetPosition();
    NO_CONST DirectX::XMFLOAT4 rotation = model->GetTransform()->GetRotation();

    // 垂直方向の移動量
    const float my = velocity.y * elapsedTime;

    // キャラクターのY軸方向となる法線ベクトル
    NO_CONST DirectX::XMFLOAT3 normal = { 0,1,0 };

    // 傾斜率初期化
    slopeRate = 0.0f;

    // 落下中
    if (my < 0.0f)
    {
        // レイの開始位置は足元より少し上
        const DirectX::XMFLOAT3 start = {
            position.x,
            position.y + stepOffset,
            position.z
        };
        // レイの終点位置は移動後の位置
        const DirectX::XMFLOAT3 end = {
            position.x,
            position.y + my,
            position.z
        };

#if 0
        // レイキャストによる地面判定
        HitResult hit = {};
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            // 法線ベクトル取得
            normal = hit.normal;

            // 傾斜率の計算
            const float normalLengthXZ = sqrtf(
                (hit.normal.x * hit.normal.x) +
                (hit.normal.z * hit.normal.z)
            );
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));

            // 地面に接地している
            //position.y = hit.position.y;
            position = hit.position;

            // 回転
            angle.y += hit.rotation.y;

            // 着地した
            if (!isGround) OnLanding();
            isGround = true;
            velocity.y = 0.0f;
        }
#else 1
        if (end.y <= 0.0f)
        {
            // バウンス中は跳ねさせる
            if (isBounce)
            {
                OnBounce();
            }
            else
            {
                position.y = 0.0f;

                // 着地した
                if (!isGround) OnLanding();
                isGround = true;
                velocity.y = 0.0f;
            }

        }
#endif
        else
        {
            // 空中に浮いている
            position.y += my;
            isGround = false;
        }
    }
    // 上昇中
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }

    // 地面の向きに沿うようにXZ軸回転
    {
        // Y軸が法線ベクトル方向に向くオイラー角回転を算出する
        const float angleX =  atan2f(normal.z, normal.y);
        const float angleZ = -atan2f(normal.x, normal.y);

        // 線形補完で滑らかに回転する
        rotation.x = Mathf::Lerp(rotation.x, angleX, 0.2f);
        rotation.z = Mathf::Lerp(rotation.z, angleZ, 0.2f);
    }

    model->GetTransform()->SetPosition(position);
    model->GetTransform()->SetRotation(rotation);
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
    NO_CONST DirectX::XMFLOAT3 position = model->GetTransform()->GetPosition();

    // 水平速力量計算
    const float velocityLengthX = sqrtf(velocity.x * velocity.x);

    if (velocityLengthX > 0.0f)
    {
        // 水平移動値
        const float mx = velocity.x * elapsedTime;

        // レイの開始位置と終点位置
        const DirectX::XMFLOAT3 start = {
            position.x,
            position.y + stepOffset,
            position.z
        };
        const DirectX::XMFLOAT3 end = {
            position.x + mx,
            position.y + stepOffset,
            position.z,
        };

        //// レイキャストによる壁判定
        //HitResult hit = {};
        //if (StageManager::Instance().RayCast(start, end, hit))
        //{
        //    // 壁までのベクトル
        //    const DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
        //    const DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
        //    const DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
        //
        //    // 壁の法線
        //    const DirectX::XMVECTOR Normal =
        //        DirectX::XMLoadFloat3(&hit.normal);
        //
        //    // 入射ベクトルを法線に射影
        //    const DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(
        //        DirectX::XMVectorNegate(Vec), Normal
        //    );
        //
        //    // 補正位置の計算        
        //    {
        //        //const DirectX::XMVECTOR MovedPosition = DirectX::XMVectorAdd(
        //        //    End, DirectX::XMVectorMultiply(Normal, Dot)
        //        //);          
        //    }
        //    // (v1 * v2) + v3
        //    const DirectX::XMVECTOR MovedPosition =
        //        DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
        //
        //    DirectX::XMFLOAT3 movedPosition = {};
        //    DirectX::XMStoreFloat3(&movedPosition, MovedPosition);
        //
        //    // 壁ずりした方向に壁がないか判定
        //    HitResult hit2 = {};
        //    if (!StageManager::Instance().RayCast(
        //        hit.position, movedPosition, hit2))
        //    {
        //        // 壁に当たらなければ補正位置に移動
        //        position.x = movedPosition.x;
        //        position.z = movedPosition.z;
        //    }
        //    else
        //    {
        //        // 壁に当たった位置に移動
        //        position.x = hit2.position.x;
        //        position.z = hit2.position.z;
        //    }
        //}
        //else
        {
            // 移動処理
            position.x += mx;
        }
    }

    model->GetTransform()->SetPosition(position);
}
