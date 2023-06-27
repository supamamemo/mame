#pragma once

#include "Block.h"
#include <string>

class GrassBlock :
    public Block
{
public:
    GrassBlock();
    GrassBlock(const char* filename);
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

    // �e�X�g�p
    DirectX::XMFLOAT4 materialColor = { 1.0f,1.0f ,1.0f ,1.0f };
    void SetMaterialColor(const DirectX::XMFLOAT4& color) { materialColor = color; }

private:
    std::string name;
};

