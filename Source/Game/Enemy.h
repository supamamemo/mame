#pragma once
#include "Character.h"
#include "StateMachine.h"
#include "CannonBallManager.h"


class Enemy : public Character
{
public:
    Enemy() {}
    virtual ~Enemy() {};

    virtual void Initialize()               = 0; // ������
    virtual void Finalize()                 = 0; // �I����
    virtual void Begin()                    = 0; // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(float elapsedTime)  = 0; // �X�V����
    virtual void End()                      = 0; // ���t���[����ԍŌ�ɌĂ΂��
    virtual void Render(float elapsedTime)  = 0; // �`�揈��
    virtual void DrawDebug()                = 0; // �f�o�b�O�`��

public:
    // �X�e�[�g�}�V��
    StateMachine* GetStateMachine()const { return stateMachine.get(); }

public:    
    std::unique_ptr<StateMachine> stateMachine{ nullptr };  // �X�e�[�g�}�V��

    CannonBallManager cannonBallManager = {};               // ��C�̒e�}�l�[�W���[

public:
    void SetMoveRight(bool r) { moveRight = r; }
    bool GetMoveRight() { return moveRight; }
private:
    bool moveRight = true;      // �E�ɐi�ނ��ǂ���
};

