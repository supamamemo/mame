#pragma once
#include "Character.h"

class CannonBallManager;

class CannonBall : public Character
{
public:
    CannonBall(DirectX::XMFLOAT3 newPosition, float directionZ, CannonBallManager* manager);
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

    CannonBallManager* manager = nullptr;

private:
    std::string name;   // Imgui�p

    int directionZ = 0;
    float offsetZ = 3;
};

