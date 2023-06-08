#pragma once

#include "Character.h"

class Player : public Character
{
public:
    Player();
    ~Player()override;

    void Initialize();      // ������
    void Finalize();        // �I����
    void Begin();           // ���t���[����ԍŏ��ɌĂ΂��
    void Update();          // �X�V����
    void End();             // ���t���[����ԍŌ�ɌĂ΂��
    void Render(float elapsedTime);          // �`�揈��
    void DrawDebug()override;

public:
    void InputMove();

    //std::unique_ptr<skinned_mesh> skinned_meshes;
};

