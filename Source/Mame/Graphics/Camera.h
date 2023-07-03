#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "../Transform.h"

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
    void PlayShake() { isShake_ = true; }      // 画面を揺らす
    void UpdateShake(const float elapsedTime); // 画面振動更新処理

public:
    Transform* GetTransform() { return &transform; }

private:
    Transform transform{};

    DirectX::XMMATRIX P{};
    DirectX::XMMATRIX V{};

    // 仮
    float length = 60.0f;
    float angle = 0.0f;
    float addAngle = 0.02f;

    float   defaultShakeTime_   = 0.075f;                 // 揺れる時間
    float   shakeTimer_         = defaultShakeTime_;    // 振動タイマー
    int     shakeState_         = 0;                    // 揺れるステート
    bool    isShake_            = false;                // 揺らすかどうか
};

