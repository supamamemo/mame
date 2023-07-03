#include "TitlePlayer.h"
#include "../Mame/Graphics/Graphics.h"

TitlePlayer::TitlePlayer()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/mameoall.fbx", true);
}

TitlePlayer::~TitlePlayer()
{
}

void TitlePlayer::Initialize()
{  
    GetTransform()->SetRotation(DirectX::XMFLOAT4(0, ToRadian(90.0f), 0, 0));
    PlayAnimation(Anim_Run, true); // 走行アニメーション再生
}

void TitlePlayer::Finalize()
{
}

void TitlePlayer::Update(const float& elapsedTime)
{
    CircularMotion(elapsedTime);

    UpdateAnimation(elapsedTime); // アニメーション更新
}

void TitlePlayer::Render(const float& elapsedTime)
{ 
    Character::Render(elapsedTime); // 共通の描画処理
}

void TitlePlayer::DrawDebug()
{
    ImGui::Begin("titlePlayer_");
    Character::DrawDebug();
    ImGui::End();
}


void TitlePlayer::CircularMotion(const float elapsedTime)
{
    Transform* transform = GetTransform();

    // 回転させる
    rotationY += rotationSpeedY * elapsedTime;  

    // 単位円（単位ベクトル）を求める
    circle.unitCircle.x = sinf(rotationY);  // sinf：中心からX軸に向かう単位円を求める
    circle.unitCircle.z = cosf(rotationY);  // cosf：中心からY軸(Z軸)に向かう単位円を求める

    // 円の中心から単位円の方向に向かって円の半径分進んだ位置を求める
    const float posX = circle.center.x + circle.unitCircle.x * circle.radius;
    const float posZ = circle.center.z + circle.unitCircle.z * circle.radius;

    // 位置を更新
    transform->SetPositionX(posX);
    transform->SetPositionZ(posZ);

    // プレイヤーを進行方向に向かせる
    const float turnVecX = sinf(rotationY + ToRadian(-90.0f)); // 進行方向に向かせるために追加で-90度曲げている
    const float turnVecZ = cosf(rotationY + ToRadian(-90.0f)); // 進行方向に向かせるために追加で-90度曲げている
    transform->SetRotationY(atan2f(turnVecX, turnVecZ));       // atan2f：座標から角度を求める

    //Turn(elapsedTime, turnVecX, turnVecZ, ToRadian(360.0f));
}


void TitlePlayer::Turn(
    const float elapsedTime, 
    NO_CONST float vx, 
    NO_CONST float vz, 
    NO_CONST float turnSpeed)
{
    // 進行ベクトルがゼロベクトルの場合は処理する必要なし
    if (vx == 0.0f && vz == 0.0f) return;

    turnSpeed *= elapsedTime;

    // 進行ベクトルを単位ベクトル化
    const float vLength = sqrtf((vx * vx) + (vz * vz));
    if (vLength < 0.001f) return;

    // 単位ベクトル化
    vx /= vLength;
    vz /= vLength;

    // 自身の回転値から前方向を求める
    const float frontX = sinf(GetTransform()->GetRotation().y);
    const float frontZ = cosf(GetTransform()->GetRotation().y);

    // 回転角を求めるため、2つの単位ベクトルの内積計算する
    const float dot = (frontX * vx) + (frontZ * vz);

    // 回転角が微小な場合は回転を行わない
    const float angle = acosf(dot); // ラジアン
    //if (fabsf(angle) <= 0.001f) return;
    if (fabsf(angle) <= 0.01f) return;

    // 内積値は-1.0~1.0で表現されており、2つの単位ベクトルの角度が
    // 小さいほど1.0に近づくという性質を利用して回転速度を調整する  
    NO_CONST float rot = 1.0f - dot;   // 補正値                    
    if (rot > turnSpeed) rot = turnSpeed;

    // 左右判定を行うために2つの単位ベクトルの外積を計算する
    const float cross = (frontZ * vx) - (frontX * vz);

    // 2Dの外積値が正の場合か負の場合によって左右判定が行える
    // 左右判定を行うことによって左右回転を選択する
    GetTransform()->AddRotationY((cross < 0.0f) ? -rot : rot);
}
