#pragma once
#include "TitleCharacter.h"

class TitleEnemyTofu : public TitleCharacter
{
public:
    TitleEnemyTofu();
    ~TitleEnemyTofu() override {};

    void Initialize();                      // ������
    void Finalize() {}                      // �I����
    void Begin()    {}                      // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float elapsedTime);   // �X�V����
    void End()      {}                      // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float elapsedTime);   // �`�揈��

    void DrawDebug() override;              // �f�o�b�O�`��

private:
    enum Animation
    {
        Anim_Walk,  // ���s
        //Anim_Turn,  // ���� 
    };

};

