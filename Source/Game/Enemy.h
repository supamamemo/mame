#pragma once
#include "Character.h"
#include "StateMachine.h"
#include "CannonBallManager.h"


class Enemy : public Character
{
public:
    Enemy() {}
    virtual ~Enemy() {};

    virtual void Initialize()                       = 0; // ������
    virtual void Finalize()                         = 0; // �I����
    virtual void Begin()                            = 0; // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(const float& elapsedTime)   = 0; // �X�V����
    virtual void End()                              = 0; // ���t���[����ԍŌ�ɌĂ΂��
    virtual void Render(const float& elapsedTime);       // �`�揈��
    virtual void DrawDebug()                        = 0; // �f�o�b�O�`��

    void OnFallDead() override;                          // �������E�����~�X�����Ƃ��ɌĂ΂��

    void Destroy();                                      // �G�l�~�[�폜

public:
    // �X�e�[�g�}�V��
    StateMachine* GetStateMachine()const { return stateMachine.get(); }

public:
    void SetMoveRight(bool r) { moveRight = r; }
    bool GetMoveRight() { return moveRight; }

public: // static�ϐ�
    static float renderLengthXLimit_;   // �G��`�悷�鋗������

public:    
    std::unique_ptr<StateMachine> stateMachine{ nullptr };  // �X�e�[�g�}�V��

    CannonBallManager cannonBallManager = {};               // ��C�̒e�}�l�[�W���[

private:
    bool moveRight = true;      // �E�ɐi�ނ��ǂ���

};

