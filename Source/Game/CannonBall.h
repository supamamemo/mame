#pragma once

#include "Character.h"

// �O���錾�i���݃C���N���[�h���Ȃ��悤�ɂ���j
class CannonBallManager;

class CannonBall : public Character
{
public:
    CannonBall(
        NO_CONST DirectX::XMFLOAT3 newPosition, 
        const float& directionZ, 
        NO_CONST CannonBallManager* manager);
    ~CannonBall();

    void Initialize();                      // ������
    void Finalize();                        // �I����
    void Begin();                           // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime);  // �X�V����
    void End();                             // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime);  // �`�揈��

    void DrawDebug() override;              // �f�o�b�O�`��

public:
    // Imgui���O���h�~�̂��߂̂�� //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui���O���h�~�̂��߂̂�� //    

    void Destroy();

    static float renderLengthXLimit_;   // �e�ۂ̕`�拗������

private:
    CannonBallManager* cannonBallManager = nullptr;

    std::string name    = {};   // Imgui�p

    float directionZ    = 0.0f;
    float offsetZ       = 3.0f;

    float lifeTimer     = 3.0f; // �����^�C�}�[
};

