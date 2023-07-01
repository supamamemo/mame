#pragma once
#include "Character.h"
#include "StateMachine.h"
#include "CannonBallManager.h"


class Enemy : public Character
{
public:
    Enemy() {}
    virtual ~Enemy() {};

    virtual void Initialize()                       = 0;    // ������
    virtual void Finalize()                         = 0;    // �I����
    virtual void Begin()                            = 0;    // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(const float& elapsedTime)   = 0;    // �X�V����
    virtual void End()                              = 0;    // ���t���[����ԍŌ�ɌĂ΂��
    virtual void Render(const float& elapsedTime);          // �`�揈��
    virtual void DrawDebug()                        = 0;    // �f�o�b�O�`��

    virtual void OnAttacked()   {};                         // �U�������Ƃ��ɌĂ΂��
    virtual void OnHitFriend()  {};                         // �����ɓ����������ɌĂ΂��

    void OnFallDead() override;                             // �������E�����~�X�����Ƃ��ɌĂ΂��

    void Destroy();                                         // �G�l�~�[�폜

    void CollisionEnemyVsPlayer();                          // �G�l�~�[�ƃv���C���[�̏Փ˔��菈��  

public:
    // �ړ�����
    void Move(const float vx, const float moveSpeed)
    {
        Character::Move(vx, moveSpeed);
    }

    // ���񏈗�
    bool Turn(
        const    float elapsedTime,
        const    float vx,
        NO_CONST float turnSpeed
    );

    // �W�����v����
    void Jump(const float jumpSpeed)
    {
        Character::Jump(jumpSpeed);
    }

public:
    // �X�e�[�g�}�V��
    StateMachine* GetStateMachine()const { return stateMachine.get(); }

    const bool GetMoveRight() const { return moveRight; }
    void SetMoveRight(const bool r) { moveRight = r; }

    // �ړI�n�̎擾�E�ݒ�
    const float& GetDestination() const { return destination_; }
    void SetDestination(const float& destination) { destination_ = destination; }

    // �ړ��͈͂̔��a�̎擾�E�ݒ�
    const float& GetMoveRangeRadius() const { return moveRangeRadius_; }
    void SetMoveRangeRadius(const float& moveRange ) { moveRangeRadius_ = moveRange; }    
    
    // �ړ��͈͂̒��SX�̎擾�E�ݒ�
    const float& GetMoveRangeCenterX() const { return moveRangeCenterX_; }
    void SetMoveRangeCenterX(const float& moveRangeCenterX ) { moveRangeCenterX_ = moveRangeCenterX; }    
    
    // ���G�����̎擾�E�ݒ�
    const float& GetSerchLength() const { return serchLength_; }
    void SetSerchLength(const float& serchLength) { serchLength_ = serchLength; }

    // �ړ�����X�̎擾�E�ݒ�
    const float& GetMoveDirectionX() const { return moveDirectionX_; }
    void SetMoveDirectionX(const float& moveDirectionX) { moveDirectionX_ = moveDirectionX; }

    // �����ɓ����������̎擾�E�ݒ�
    const bool GetIsHitFriend() const { return isHitFrined_; }
    void SetIsHitFriend(const bool isHitFriend) { isHitFrined_ = isHitFriend; }

public: // static�ϐ�
    static float renderLengthXLimit_;   // �G��`�悷�鋗������

public:    
    std::unique_ptr<StateMachine> stateMachine{ nullptr };  // �X�e�[�g�}�V��

    CannonBallManager cannonBallManager = {};               // ��C�̒e�}�l�[�W���[

protected:
    float destination_      = 0.0f;     // �ړI�n
    float moveRangeCenterX_ = 0.0f;     // �ړ��͈͂̒��SX
    float moveRangeRadius_  = 6.0f;     // �ړ��͈͂̔��a
    float serchLength_      = 6.0f;     // ���G�ł��鋗��
    float moveDirectionX_   = 1.0f;     // �ړ�����X  

    bool  isHitFrined_      = false;    // �����ɓ���������

private:
    bool moveRight          = true; // �E�ɐi�ނ��ǂ���
};

