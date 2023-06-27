#pragma once

#include "Enemy.h"

enum class BossAnimation
{
    Idle,
    Find,
    Attack,
    Recoil,
};

class Boss : public Enemy
{
public:
    Boss();
    //Boss(const char* filename); // ���ł�

    ~Boss()override;

    void Initialize();              // ������
    void Finalize();                // �I����
    void Begin();                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(float elapsedTime); // �X�V����
    void End();                     // ���t���[����ԍŌ�ɌĂ΂��
    void Render(float elapsedTime); // �`�揈��
    void DrawDebug()override;



public:
    // Imgui���O���h�~�̂��߂̂�� //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui���O���h�~�̂��߂̂�� //    


private:
    std::string name;   // Imgui�p
};

