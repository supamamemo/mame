#include "EnemyManager.h"
#include "../Game/Common.h"
#include "../Game/BossStateDerived.h"
#include "../Mame/AudioManager.h"


void EnemyManager::Initialize()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->Initialize();
    }
}


void EnemyManager::Finalize()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->Finalize();
    }
}


void EnemyManager::Begin()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->Begin();
    }
}


// �X�V����
void EnemyManager::Update(const float& elapsedTime)
{
    // �X�V
    for (Enemy*& enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    // �j������
    {
        for (Enemy* enemy : removes)
        {
            // vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜
            std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

            // std::vector�ŊǗ�����Ă���v�f���폜����ɂ�erase()�֐����g�p����
            // (�j�����X�g�̃|�C���^����C�e���[�^�[���������Aerase�֐��ɓn��)
            if (it != enemies.end())
            {
                enemies.erase(it);
            }

            // �G�̔j������
            SafeDelete(enemy);
        }
        // �j�����X�g���N���A
        removes.clear();
    }


    // �G�l�~�[���m�̏Փ˔��菈��
    CollisionEnemyVsEnemy();
}


void EnemyManager::End()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->End();
    }
}


// �`�揈��
void EnemyManager::Render(const float& elapsedTime)
{
    for (Enemy*& enemy : enemies)
    {
        enemy->Render(elapsedTime);
    }
}


// �G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}


// �G�l�~�[�폜
void EnemyManager::Remove(Enemy* enemy)
{
    // �j�����X�g�ɒǉ�
    removes.insert(enemy);
}


// �G�l�~�[�S�폜
void EnemyManager::Clear()
{
    for (Enemy*& enemy : enemies)
    {
        SafeDelete(enemy);
    }
    enemies.clear();
    enemies.shrink_to_fit();    // vector�̗]���ȃ��������������֐�(C++11)
}


// �f�o�b�O
void EnemyManager::DrawDebug()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->DrawDebug();
    }
}

void EnemyManager::AllKill()
{
    for (Enemy*& enemy : enemies)
    {
        enemy->ApplyDamage(999, 1.0f);

        // �q�b�gSE���Đ�
        if (enemy->IsInLengthPlayer(Enemy::playSELengthXLimit_))
        {
            AudioManager& audioManager = AudioManager::Instance();
            audioManager.StopSE(SE::PL_BounceHit);
            audioManager.PlaySE(SE::PL_BounceHit, false, true);
        }
    }
}


// �G�l�~�[���m�̏Փ˔��菈��
void EnemyManager::CollisionEnemyVsEnemy()
{
    NO_CONST bool isHitX_A = false;
    NO_CONST bool isHitX_B = false;

    NO_CONST bool isHitY_A = false;
    NO_CONST bool isHitY_B = false;

    EnemyManager& enemyManager = EnemyManager::Instance();
    const int enemyCount       = enemyManager.GetEnemyCount();
    for (int a = 0; a < enemyCount; ++a)
    {
        Enemy* enemyA = enemyManager.GetEnemy(a);
        if (enemyA->GetHealth() <= 0) continue;

        //// A�̖����̏�ɏ�������Ă��邩�t���O���ŏ��Ƀ��Z�b�g����
        //enemyA->SetIsOnFriend(false);

        // a�ȍ~�̓G�Ɣ�����s���ia�ȑO�͂��łɔ���ς݂̂��߁j
        for (int b = a + 1; b < enemyCount; ++b)
        {
            Enemy* enemyB = enemyManager.GetEnemy(b);
            if (enemyB->GetHealth() <= 0) continue;

            //// B�̖����̏�ɏ�������Ă��邩�t���O���ŏ��Ƀ��Z�b�g����
            //enemyB->SetIsOnFriend(false);

            if (Collision::IntersectAABBVsAABB(enemyA->aabb_, enemyB->aabb_))
            {
                // �G�l�~�[A���ԓ����ŒǐՒ��Ȃ�G�l�~�[B�𐁂���΂�
                if (enemyA->enemyType_ == EnemyType::RedTofu &&
                    enemyB->enemyType_ == EnemyType::Tofu)
                {
                    if (enemyA->GetStateMachine()->GetStateIndex() == static_cast<int>(RED_TOFU::STATE::Track))
                    {
                        enemyB->SetMoveDirectionX(enemyA->GetMoveDirectionX());
                        enemyB->ApplyDamage(1, 1.0f);

                        // �q�b�gSE���Đ�
                        if (enemyB->IsInLengthPlayer(Enemy::playSELengthXLimit_))
                        {
                            AudioManager& audioManager = AudioManager::Instance();
                            audioManager.StopSE(SE::PL_BounceHit);              // �o�E���X�q�b�gSE��~
                            audioManager.PlaySE(SE::PL_BounceHit, false, true); // �o�E���X�q�b�gSE�Đ�
                        }
                    }
                }
                // �G�l�~�[B���ԓ����ŒǐՒ��Ȃ�G�l�~�[A�𐁂���΂�
                if (enemyB->enemyType_ == EnemyType::RedTofu &&
                    enemyA->enemyType_ == EnemyType::Tofu)
                {
                    if (enemyB->GetStateMachine()->GetStateIndex() == static_cast<int>(RED_TOFU::STATE::Track))
                    {
                        enemyA->SetMoveDirectionX(enemyB->GetMoveDirectionX());
                        enemyA->ApplyDamage(1, 1.0f);

                        // �q�b�gSE���Đ�
                        if (enemyA->IsInLengthPlayer(Enemy::playSELengthXLimit_))
                        {
                            AudioManager& audioManager = AudioManager::Instance();
                            audioManager.StopSE(SE::PL_BounceHit);              // �o�E���X�q�b�gSE��~
                            audioManager.PlaySE(SE::PL_BounceHit, false, true); // �o�E���X�q�b�gSE�Đ�
                        }
                    }
                }

                // �㉺���E�̂��ꂼ��d�Ȃ��Ă���l�����߂�
                // �������}�C�i�X�ł��v���X�̋��������߂���悤�ɐ�Βl�ɂ���
                const float overlapUp     = fabsf(enemyA->aabb_.min.y - enemyB->aabb_.max.y); // �ォ��߂荞��ł���
                const float overlapBottom = fabsf(enemyA->aabb_.max.y - enemyB->aabb_.min.y); // ������߂荞��ł���
                const float overlapRight  = fabsf(enemyA->aabb_.min.x - enemyB->aabb_.max.x); // �E����߂荞��ł���
                const float overlapLeft   = fabsf(enemyA->aabb_.max.x - enemyB->aabb_.min.x); // ������߂荞��ł���
                // ��ԏd�Ȃ��Ă��Ȃ��l�����߂�
                const float overlapY      = (std::min)(overlapUp, overlapBottom);
                const float overlapX      = (std::min)(overlapRight, overlapLeft);
                //const float overlap       = (std::min)(overlapY, overlapX);

                // Y���ɏd�Ȃ��Ă���ꍇ
                if (overlapY <= overlapX)
                {
                    isHitY_A = true;
                    isHitY_B = true;

                    // �ォ��߂荞��ł���ꍇ
                    if (overlapUp <= overlapBottom)
                    {
                        enemyA->SetVelocityY(0.0f); // Y���x�����Z�b�g

                        // �d�Ȃ��Ă��镪�����ʒu���C��
                        if (overlapUp != 0.0f)
                        {
                            enemyA->GetTransform()->AddPositionY(overlapUp);
                            //enemyA->GetTransform()->AddPositionY( overlapUp * 0.5f);
                            //enemyB->GetTransform()->AddPositionY(-overlapUp * 0.5f);
                        }

                        // �����̏�ɏ�������Ă���
                        enemyA->SetIsOnFriend(true);
                    }
                    // ������߂荞��ł���ꍇ
                    else
                    {
                        enemyB->SetVelocityY(0.0f); // Y���x�����Z�b�g

                        // �d�Ȃ��Ă��镪�����ʒu���C��
                        if (overlapBottom != 0.0f)
                        {
                            //enemyA->GetTransform()->AddPositionY(-overlapBottom);
                            enemyB->GetTransform()->AddPositionY(overlapBottom);
                            //enemyA->GetTransform()->AddPositionY(-overlapBottom * 0.5f);
                            //enemyB->GetTransform()->AddPositionY( overlapBottom * 0.5f);
                        }

                        // �����̏�ɏ�������Ă���
                        enemyB->SetIsOnFriend(true);
                    }
                }
                // X���ɏd�Ȃ��Ă���ꍇ
                else
                {
                    // �d�Ȃ肪�Ȃ���ΏC�����Ȃ�
                    if (overlapX == 0.0f) continue;

                    enemyA->SetVelocityX(0.0f); // X���x�����Z�b�g
                    enemyB->SetVelocityX(0.0f); // X���x�����Z�b�g

                    isHitX_A = true;
                    isHitX_B = true;

                    // �E����߂荞��ł���ꍇ
                    if (overlapX == overlapRight)
                    {
                        // �d�Ȃ��Ă��镪�����ʒu���C��
                        //enemyA->GetTransform()->AddPositionX(overlapRight + 0.1f);
                        enemyA->GetTransform()->AddPositionX(( overlapRight +  0.05f) * 0.5f); // �������񃋁[�v�h�~�̂��߂ɏ�����������
                        enemyB->GetTransform()->AddPositionX((-overlapRight + -0.05f) * 0.5f); // �������񃋁[�v�h�~�̂��߂ɏ�����������
                    }
                    // ������߂荞��ł���ꍇ
                    else if (overlapX == overlapLeft)
                    {
                        // �d�Ȃ��Ă��镪�����ʒu���C��
                        //enemyA->GetTransform()->AddPositionX(-overlapLeft + -0.1f);
                        enemyA->GetTransform()->AddPositionX((-overlapLeft + -0.05f) * 0.5f); // �������񃋁[�v�h�~�̂��߂ɏ�����������
                        enemyB->GetTransform()->AddPositionX(( overlapLeft +  0.05f) * 0.5f); // �������񃋁[�v�h�~�̂��߂ɏ�����������
                    }

                    // �����瓖�����Ă����疡���ɓ����������̏������s��
                    //enemyA->OnHitFriend();
                    //enemyB->OnHitFriend();
                    if (!enemyA->GetIsHitFriend()) enemyA->OnHitFriend();
                    if (!enemyB->GetIsHitFriend()) enemyB->OnHitFriend();
                    enemyA->SetIsHitFriend(true);
                    enemyB->SetIsHitFriend(true);
                }

                // �����߂����AABB�̍ŏ����W�ƍő���W���X�V
                enemyA->UpdateAABB();
                enemyB->UpdateAABB();
            }

            if (!isHitX_B)
            {
                enemyB->SetIsHitFriend(false);
            }                   
            else
            { 
                isHitX_B = false;
            }            
            
            //if (!isHitY_B)
            //{
            //    enemyB->SetIsOnFriend(false);
            //}                   
            //else
            //{ 
            //    isHitY_B = false;
            //}
        }

        if (!isHitX_A)
        {
            enemyA->SetIsHitFriend(false);
        }        
        else
        {
            isHitX_A = false;
        }

        //if (!isHitY_A)
        //{
        //    enemyA->SetIsOnFriend(false);
        //}
        //else
        //{
        //    isHitY_A = false;
        //}
    }
}
