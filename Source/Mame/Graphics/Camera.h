#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "../Transform.h"
#include "../../Game/Common.h"

enum class ShakeType
{
    None = -1,
    HorizontalShake,    // 横揺れ
    VerticalShake       // 縦揺れ
};

class Camera
{
private: // シングルトン化
    Camera()  {}
    ~Camera() {}

public:
    // 唯一のインスタンス取得
    static Camera& Instance()
    {
        static Camera camera;
        return camera;
    }

    void Initialize();
    void Update(float elapsedTime);

    void UpdateTitle(float elapsedTime);
    void UpdateTutorial(float elapsedTime, int state);
    void UpdateBoss(const float elapsedTime);

    void SetPerspectiveFov(ID3D11DeviceContext* dc);
    void SetPerspectiveFovTitle(ID3D11DeviceContext* dc);
    //void SetPerspectiveFov(ID3D11DeviceContext* dc, float fovY, float aspect, float nearZ, float farZ);
   
    DirectX::XMMATRIX GetP() { return P; }
    DirectX::XMMATRIX GetV() { return V; }

    void DrawDebug();

    struct view_camera
    {
        DirectX::XMFLOAT3 eye{ 0.0f,0.0f,-10.0f };
        DirectX::XMFLOAT3 focus{ 0.0f,0.0f,0.0f };
        DirectX::XMFLOAT3 up{ 0.0f,1.0f,0.0f };
    }camera;

    void Reset();
    void DebugMoveCamera();

    void SetAngle(float a) { angle = a; }

#ifdef _DEBUG
    void SetDebugCamera();
    void SetDebugCamera1();
#endif // _DEBUG

public:
    // 画面を揺らす
    void PlayShake(const ShakeType shakeType, const int loopCount = 2)
    {
        isShake_                = true;
        currentShakeLoopCount_  = 0;            // 現在のループ回数をリセット
        shakeLoopCount_         = loopCount;    // 繰り返す回数を設定
        shakeType_              = shakeType;    // シェイクの種類を設定
    }
    void UpdateShake(const float elapsedTime); // 画面振動更新処理

public:
    Transform* GetTransform() { return &transform; }

    void SetTutorialState() { tutorialState = 0; }
    void SetCameraMoveY() { cameraMoveY = 0; }

private:
    Transform transform{};

    DirectX::XMMATRIX P{};
    DirectX::XMMATRIX V{};

    // 仮
    float length   = 60.0f;
    float angle    = 0.0f;
    float addAngle = ToRadian(15.0f);

    ShakeType   shakeType_              = ShakeType::None;      // シェイクの種類
    float       defaultShakeTime_       = 0.025f;                // 揺れる時間(速度調整)
    float       shakeTimer_             = defaultShakeTime_;    // 振動タイマー
    int         shakeState_             = 0;                    // 揺れるステート
    int         currentShakeLoopCount_  = 0;                    // 現在のシェイクカウント
    int         shakeLoopCount_         = 0;                    // 設定するシェイクカウント
    bool        isShake_                = false;                // 揺らすかどうか

    // StagePlains用
    int cameraMoveY = 0;
    bool cameraUp = false; // カメラ上昇するか
    bool cameraDown = false;

    // tutorial用
    int tutorialState = 0;

private:
    // チュートリアル用
    enum STATE
    {
        MoveReception,  // 移動受付
        JumpReception,  // ジャンプ受付
        HipDorop,
        Run,
        Free,
    };
};

