#pragma once

#include "Block.h"

class GrassBlock :
    public Block
{
public:
    GrassBlock();
    ~GrassBlock()override;

    void Initialize();      // ������
    void Finalize();        // �I����
    void Begin();           // ���t���[����ԍŏ��ɌĂ΂��
    void Update();          // �X�V����
    void End();             // ���t���[����ԍŌ�ɌĂ΂��
    void Render(float elapsedTime);          // �`�揈��
    void DrawDebug()override;

    
};

