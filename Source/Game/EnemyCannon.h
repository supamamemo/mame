#pragma once
#include "Enemy.h"
#include "CannonBallManager.h"

// ��C
class EnemyCannon : public Enemy
{
public:
    EnemyCannon();
    ~EnemyCannon() override;

    void Initialize()               override; // ������
    void Finalize()                 override; // �I����
    void Begin()                    override; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(float elapsedTime)  override; // �X�V����
    void End()                      override; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(float elapsedTime)  override; // �`�揈��
    void DrawDebug()                override;

public:
    // Imgui���O���h�~�̂��߂̂�� //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui���O���h�~�̂��߂̂�� //

private:
    std::string name = {};};

