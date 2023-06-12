#pragma once

#include "Block.h"
#include <string>

class GrassBlock :
    public Block
{
public:
    GrassBlock();
    ~GrassBlock()override;

    void Initialize(DirectX::XMFLOAT3 pos);      // ������
    void Finalize();        // �I����
    void Begin();           // ���t���[����ԍŏ��ɌĂ΂��
    void Update();          // �X�V����
    void End();             // ���t���[����ԍŌ�ɌĂ΂��
    void Render(float elapsedTime);          // �`�揈��
    void DrawDebug()override;

public:
    static int nameNum;

    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }

private:
    std::string name;
};

