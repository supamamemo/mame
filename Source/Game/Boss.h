#pragma once

#include "Character.h"

class Boss : public Character
{
public:
    Boss();
    ~Boss()override;

    void Initialize();      // ������
    void Finalize();        // �I����
    void Begin();           // ���t���[����ԍŏ��ɌĂ΂��
    void Update();          // �X�V����
    void End();             // ���t���[����ԍŌ�ɌĂ΂��
    void Render(float elapsedTime);          // �`�揈��
    void DrawDebug()override;
};

