#pragma once
#include "Character.h"
#include "StateMachine.h"
#include "CannonBallManager.h"

enum class EnemyType
{
    Tofu,       // ����
    RedTofu,    // �ԓ���
};

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

    bool IsInLengthPlayer(const float lengthXLimit);        // �v���C���[���������ɂ��邩���肷��

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
    
    // ���G����X�̎擾�E�ݒ�
    const float& GetSerchLengthX() const { return searchLengthX_; }
    void SetSerchLengthX(const float& searchLengthX) { searchLengthX_ = searchLengthX; }       
    // ���G����Y�̎擾�E�ݒ�
    const float& GetSerchLengthY() const { return searchLengthY_; }
    void SetSerchLengthY(const float& searchLengthY) { searchLengthY_ = searchLengthY; }

    // �ړ�����X�̎擾�E�ݒ�
    const float& GetMoveDirectionX() const { return moveDirectionX_; }
    void SetMoveDirectionX(const float& moveDirectionX) { moveDirectionX_ = moveDirectionX; }

    // �v���C���[�ɓ����������̎擾�E�ݒ�
    const bool GetIsHitPlayer() const { return isHitPlayer_; }
    void SetIsHitPlayer(const bool isHitPlayer) { isHitPlayer_ = isHitPlayer; }

    // �����ɓ����������̎擾�E�ݒ�
    const bool GetIsHitFriend() const { return isHitFriend_; }
    void SetIsHitFriend(const bool isHitFriend) { isHitFriend_ = isHitFriend; }

    // �����̏�ɏ�������Ă��邩�̎擾�E�ݒ�
    const bool GetIsOnFriend() const { return isOnFriend_; }
    void SetIsOnFriend(const bool isOnFriend) { isOnFriend_ = isOnFriend; }

public: // static�ϐ�
    static float renderLengthXLimit_;   // �G��`�悷�鋗������
    static float playSELengthXLimit_;   // SE���Đ����鋗������

public:    
    std::unique_ptr<StateMachine> stateMachine{ nullptr };  // �X�e�[�g�}�V��

    CannonBallManager cannonBallManager = {};               // ��C�̒e�}�l�[�W���[

    EnemyType enemyType_    = {};                           // �G�̎��(���ʗp)

protected:
    float destination_      =    0.0f;     // �ړI�n
    float moveRangeCenterX_ =    0.0f;     // �ړ��͈͂̒��SX
    float moveRangeRadius_  =    4.0f;     // �ړ��͈͂̔��a
    float searchLengthX_    =    4.0f;     // X���ō��G�ł��鋗��
    float searchLengthY_    =    2.0f;     // Y���ō��G�ł��鋗��
    float moveDirectionX_   =   -1.0f;     // �ړ�����X�i�f�t�H���g�͍������j

    bool  isHitPlayer_      =   false;     // �v���C���[�ɓ���������
    bool  isHitFriend_      =   false;     // �����ɓ���������
    bool  isOnFriend_       =   false;     // �����̏�ɏ�������Ă��邩

private:
    bool moveRight          =   true; // �E�ɐi�ނ��ǂ���
};

