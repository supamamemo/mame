#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "../Transform.h"
#include "../../Game/Common.h"

enum class ShakeType
{
    None = -1,
    HorizontalShake,    // ���h��
    VerticalShake       // �c�h��
};

class Camera
{
private: // �V���O���g����
    Camera()  {}
    ~Camera() {}

public:
    // �B��̃C���X�^���X�擾
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
    // ��ʂ�h�炷
    void PlayShake(const ShakeType shakeType, const int loopCount = 2)
    {
        isShake_                = true;
        currentShakeLoopCount_  = 0;            // ���݂̃��[�v�񐔂����Z�b�g
        shakeLoopCount_         = loopCount;    // �J��Ԃ��񐔂�ݒ�
        shakeType_              = shakeType;    // �V�F�C�N�̎�ނ�ݒ�
    }
    void UpdateShake(const float elapsedTime); // ��ʐU���X�V����

public:
    Transform* GetTransform() { return &transform; }

    void SetTutorialState() { tutorialState = 0; }
    void SetCameraMoveY() { cameraMoveY = 0; }

private:
    Transform transform{};

    DirectX::XMMATRIX P{};
    DirectX::XMMATRIX V{};

    // ��
    float length   = 60.0f;
    float angle    = 0.0f;
    float addAngle = ToRadian(15.0f);

    ShakeType   shakeType_              = ShakeType::None;      // �V�F�C�N�̎��
    float       defaultShakeTime_       = 0.025f;                // �h��鎞��(���x����)
    float       shakeTimer_             = defaultShakeTime_;    // �U���^�C�}�[
    int         shakeState_             = 0;                    // �h���X�e�[�g
    int         currentShakeLoopCount_  = 0;                    // ���݂̃V�F�C�N�J�E���g
    int         shakeLoopCount_         = 0;                    // �ݒ肷��V�F�C�N�J�E���g
    bool        isShake_                = false;                // �h�炷���ǂ���

    // StagePlains�p
    int cameraMoveY = 0;
    bool cameraUp = false; // �J�����㏸���邩
    bool cameraDown = false;

    // tutorial�p
    int tutorialState = 0;

private:
    // �`���[�g���A���p
    enum STATE
    {
        MoveReception,  // �ړ���t
        JumpReception,  // �W�����v��t
        HipDorop,
        Run,
        Free,
    };
};

