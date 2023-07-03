#include "TitlePlayer.h"
#include "../Mame/Graphics/Graphics.h"

TitlePlayer::TitlePlayer()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/mameoall.fbx", true);
}

TitlePlayer::~TitlePlayer()
{
}

void TitlePlayer::Initialize()
{  
    GetTransform()->SetRotation(DirectX::XMFLOAT4(0, ToRadian(90.0f), 0, 0));
    PlayAnimation(Anim_Run, true); // ���s�A�j���[�V�����Đ�
}

void TitlePlayer::Finalize()
{
}

void TitlePlayer::Update(const float& elapsedTime)
{
    CircularMotion(elapsedTime);

    UpdateAnimation(elapsedTime); // �A�j���[�V�����X�V
}

void TitlePlayer::Render(const float& elapsedTime)
{ 
    Character::Render(elapsedTime); // ���ʂ̕`�揈��
}

void TitlePlayer::DrawDebug()
{
    ImGui::Begin("titlePlayer_");
    Character::DrawDebug();
    ImGui::End();
}


void TitlePlayer::CircularMotion(const float elapsedTime)
{
    Transform* transform = GetTransform();

    // ��]������
    rotationY += rotationSpeedY * elapsedTime;  

    // �P�ʉ~�i�P�ʃx�N�g���j�����߂�
    circle.unitCircle.x = sinf(rotationY);  // sinf�F���S����X���Ɍ������P�ʉ~�����߂�
    circle.unitCircle.z = cosf(rotationY);  // cosf�F���S����Y��(Z��)�Ɍ������P�ʉ~�����߂�

    // �~�̒��S����P�ʉ~�̕����Ɍ������ĉ~�̔��a���i�񂾈ʒu�����߂�
    const float posX = circle.center.x + circle.unitCircle.x * circle.radius;
    const float posZ = circle.center.z + circle.unitCircle.z * circle.radius;

    // �ʒu���X�V
    transform->SetPositionX(posX);
    transform->SetPositionZ(posZ);

    // �v���C���[��i�s�����Ɍ�������
    const float turnVecX = sinf(rotationY + ToRadian(-90.0f)); // �i�s�����Ɍ������邽�߂ɒǉ���-90�x�Ȃ��Ă���
    const float turnVecZ = cosf(rotationY + ToRadian(-90.0f)); // �i�s�����Ɍ������邽�߂ɒǉ���-90�x�Ȃ��Ă���
    transform->SetRotationY(atan2f(turnVecX, turnVecZ));       // atan2f�F���W����p�x�����߂�

    //Turn(elapsedTime, turnVecX, turnVecZ, ToRadian(360.0f));
}


void TitlePlayer::Turn(
    const float elapsedTime, 
    NO_CONST float vx, 
    NO_CONST float vz, 
    NO_CONST float turnSpeed)
{
    // �i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    if (vx == 0.0f && vz == 0.0f) return;

    turnSpeed *= elapsedTime;

    // �i�s�x�N�g����P�ʃx�N�g����
    const float vLength = sqrtf((vx * vx) + (vz * vz));
    if (vLength < 0.001f) return;

    // �P�ʃx�N�g����
    vx /= vLength;
    vz /= vLength;

    // ���g�̉�]�l����O���������߂�
    const float frontX = sinf(GetTransform()->GetRotation().y);
    const float frontZ = cosf(GetTransform()->GetRotation().y);

    // ��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��όv�Z����
    const float dot = (frontX * vx) + (frontZ * vz);

    // ��]�p�������ȏꍇ�͉�]���s��Ȃ�
    const float angle = acosf(dot); // ���W�A��
    //if (fabsf(angle) <= 0.001f) return;
    if (fabsf(angle) <= 0.01f) return;

    // ���ϒl��-1.0~1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
    // �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����  
    NO_CONST float rot = 1.0f - dot;   // �␳�l                    
    if (rot > turnSpeed) rot = turnSpeed;

    // ���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
    const float cross = (frontZ * vx) - (frontX * vz);

    // 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    // ���E������s�����Ƃɂ���č��E��]��I������
    GetTransform()->AddRotationY((cross < 0.0f) ? -rot : rot);
}
