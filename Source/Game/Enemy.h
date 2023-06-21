#pragma once
#include "Character.h"
#include "StateMachine.h"

class Enemy : public Character
{
public:
    Enemy() {}
    virtual ~Enemy() {};

    virtual void Initialize() = 0;              // ������
    virtual void Finalize() = 0;                // �I����
    virtual void Begin() = 0;                   // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(float elapsedTime) = 0; // �X�V����
    virtual void End() = 0;                     // ���t���[����ԍŌ�ɌĂ΂��
    virtual void Render(float elapsedTime) = 0; // �`�揈��
    virtual void DrawDebug() = 0;

public:
    // �A�j���[�V�����Đ��ݒ�
    void PlayAnimation(const int& index, const bool& loop);

    // �A�j���[�V�����X�V����
    void UpdateAnimation(const float& elapsedTime);

    // �A�j���[�V�������Đ������ǂ���
    bool IsPlayAnimation() const;

    // �X�e�[�g�}�V��
    StateMachine* GetStateMachine()const { return stateMachine.get(); }

    
    std::unique_ptr<StateMachine> stateMachine{ nullptr };  // �X�e�[�g�}�V��


};

