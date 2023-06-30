#include "EnemyCannon.h"

#include "../Mame/Graphics/Graphics.h"

#include "BossStateDerived.h"

#include "PlayerManager.h"

int EnemyCannon::nameNum = 0;

// �R���X�g���N�^
EnemyCannon::EnemyCannon()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_cannon.fbx", true);

    // �X�e�[�g�}�V�����Z�b�g
    stateMachine.reset(new StateMachine);

    GetStateMachine()->RegisterState(new CANNON::IdleState(this));
    GetStateMachine()->RegisterState(new CANNON::AttackState(this));

    GetStateMachine()->SetState(static_cast<int>(CANNON::STATE::Idle));


    // imgui���O���Ԃ肪�N���Ȃ��悤��...
    name = "EnemyCannon" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

// �f�X�g���N�^
EnemyCannon::~EnemyCannon()
{
}

// ������
void EnemyCannon::Initialize()
{
}

// �I����
void EnemyCannon::Finalize()
{
}

// Update�̑O�ɌĂ΂��
void EnemyCannon::Begin()
{
}

// �X�V����
void EnemyCannon::Update(const float& elapsedTime)
{
    // cannonBallManager
    cannonBallManager.Update(elapsedTime);

    if (stateMachine) GetStateMachine()->Update(elapsedTime);

    // AABB�X�V����
    UpdateAABB();

    CollisionCannonBallVsPlayer(); // �G�l�~�[�e�ۂƃv���C���[�̏Փˏ���
}

// Update�̌�ɌĂ΂��
void EnemyCannon::End()
{
}

// �`�揈��
void EnemyCannon::Render(const float& elapsedTime)
{
    Enemy::Render(elapsedTime);

    // cannonBallManager
    cannonBallManager.Render(elapsedTime);

}

// debug�p
void EnemyCannon::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin(GetName());

    model->GetTransform()->DrawDebug();

    if (stateMachine) GetStateMachine()->DrawDebug();

    cannonBallManager.DrawDebug();

    ImGui::End();
#endif // USE_IMGUI
}


void EnemyCannon::CollisionCannonBallVsPlayer()
{
    const int cannonBallCount = cannonBallManager.GetCannonBallCount();

    bool isHit = false;

    for (int i = 0; i < cannonBallCount; ++i)
    {
        CannonBall* cannonBall = cannonBallManager.GetCannonBall(i);

        const Collision::AABB& playerAABB = PlayerManager::Instance().GetPlayer()->aabb_;
        if (Collision::IntersectAABBVsAABB(cannonBall->aabb_, playerAABB))
        {
            isHit = true;
            // AABB1�������o��
            //if (outPosition.max.x - outPosition.min.x < outPosition.max.y - outPosition.min.y)
            //{
            //    if (outPosition.max.x - box1.min.x < box1.max.x - outPosition.min.x)
            //    {
            //        box1.max.x = outPosition.min.x;
            //        GetTransform()->AddPosition(DirectX::XMFLOAT3(outPosition.min.x, 0, 0));
            //    }
            //    else
            //    {
            //        box1.min.x = outPosition.max.x;
            //        GetTransform()->AddPosition(DirectX::XMFLOAT3(outPosition.max.x, 0, 0));
            //    }
            //}
            //else
            //{
            //    if (outPosition.max.y - box1.min.y < box1.max.y - outPosition.min.y)
            //    {
            //        //box1.max.y = outPosition.min.y;
            //        GetTransform()->AddPosition(DirectX::XMFLOAT3(0, outPosition.min.y, 0));
            //    }
            //    else
            //    {
            //        //box1.min.y = outPosition.max.y;
            //        GetTransform()->AddPosition(DirectX::XMFLOAT3(0, outPosition.max.y, 0));
            //    }
            //}
            //if (outPosition.max.z - box1.min.z < box1.max.z - outPosition.min.z)
            //    box1.max.z = outPosition.min.z;
            //else
            //    box1.min.z = outPosition.max.z;   
        }
    }
}
