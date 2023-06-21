#pragma once

#include "Character.h"
#include "StateMachine.h"


enum class BossAnimation
{
    Idle,
    Find,
    Attack,
    Recoil,
};

class Boss : public Character
{
public:
    Boss();
    Boss(const char* filename); // ���ł�

    ~Boss()override;

    void Initialize();              // ������
    void Finalize();                // �I����
    void Begin();                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(float elapsedTime); // �X�V����
    void End();                     // ���t���[����ԍŌ�ɌĂ΂��
    void Render(float elapsedTime); // �`�揈��
    void DrawDebug()override;

    // �A�j���[�V�����Đ��ݒ�
    void PlayAnimation(const int& index, const bool& loop);

    // �A�j���[�V�����X�V����
    void UpdateAnimation(const float& elapsedTime);

    // �A�j���[�V�������Đ������ǂ���
    bool IsPlayAnimation() const;

public:
    // Imgui���O���h�~�̂��߂̂�� //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui���O���h�~�̂��߂̂�� //

    // �X�e�[�g�}�V��
    StateMachine* GetStateMachine()const { return stateMachine.get(); }

    // ����̓e�X�g�p�Ŏg���Ă邾���Ȃ̂ŁA�A��ł���Ȃ��Ȃ邩��
    DirectX::XMFLOAT4 materialColor = { 1,1,1,1 };
    void SetMaterialColor(DirectX::XMFLOAT4 color) { materialColor = color; }

    

private:
    std::string name;   // Imgui�p

    std::unique_ptr<StateMachine> stateMachine{ nullptr };  // �X�e�[�g�}�V��
};

