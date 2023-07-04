#pragma once

#include "TitleCharacter.h"

// �^�C�g���p�v���C���[
class TitlePlayer : public TitleCharacter
{
public:
    TitlePlayer();
    ~TitlePlayer() override {};

    void Initialize();                      // ������
    void Finalize() {};                     // �I����
    void Begin()    {}                      // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float elapsedTime);   // �X�V����
    void End()      {}                         // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float elapsedTime);   // �`�揈��

    void DrawDebug() override;              // �f�o�b�O�`��

private:
    enum Animation
    {
        //Anim_Idle,      // �ҋ@
        //Anim_Dash,      // �_�b�V���i�ꎞ�I�ȋ}�����j
        Anim_Run = 2,       // ���s
        //Anim_Break,     // �u���[�L
        //Anim_JumpInit,  // �W�����v�J�n
        //Anim_Jump,      // �W�����v
        //Anim_Fall,      // ����
        //Anim_JumpEnd,   // ���n�i�W�����v�I���j
        //Anim_HipDrop,   // �q�b�v�h���b�v
        //Anim_Walk,      // ���s
        //Anim_Max,       // �A�j���[�V�����ő吔
    };

};