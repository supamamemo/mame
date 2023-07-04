#pragma once

#include "../Mame/Graphics/Model.h"
#include <memory>

// �^�C�g���p�L�����N�^�[
class TitleCharacter
{
public:
    TitleCharacter() {}
    virtual ~TitleCharacter() {}

    virtual void Render(const float elapsedTime);   // �`�揈��
    virtual void DrawDebug();                       // �f�o�b�O�`��

public:  
    Transform* GetTransform() const { return model_->GetTransform(); } // �g�����X�t�H�[���擾

    // ��]�l�ݒ�
    void SetRotationY(const float rotationY) { rotationY_ = rotationY; };

protected:
    // �~�^���s������
    virtual void CircularMotion(const float elapsedTime);

    virtual void Turn(
        const float elapsedTime,
        NO_CONST float vx,
        NO_CONST float vz,
        NO_CONST float turnSpeed
    );

protected:
    // XMFLOAT�̃����o�ϐ���Y��Z�ŕ���킵���̂ō쐬
    struct Vector2D_XZ
    {
        float x = 0.0f;
        float z = 0.0f;
    };

    // �~
    struct Circle
    {
        Vector2D_XZ center = { 0,0 };   // �~�̒��S
        float       radius = 0.0f;      // �~�̔��a
    };

protected:
    std::unique_ptr<Model> model_ = nullptr;        // ���f��

    Circle  circle_         = { { 0,0 }, 20.0f };   // �~�^���̉~
    float   rotationY_      = ToRadian(0.0f);       // ��]�l(�g�����X�t�H�[���̊p�x�𒼐ڂ�����ƈړ����x���e�����󂯂Ĕ����ɂȂ��Ă��܂��̂ŕʂŗp�ӂ��Ă���)
    float   rotationSpeedY_ = ToRadian(-45.0f);     // ��]���xY�i�ړ����x�j
};

