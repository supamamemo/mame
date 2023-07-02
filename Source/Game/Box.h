#pragma once

#include <string>
#include "../Mame/Graphics/Model.h"


class Box
{
public:
    Box(const char* filename);
    ~Box() {}

    void Initialize();                        // ������
    void Finalize();                          // �I����
    void Begin() {}                             // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime);    // �X�V����
    void End() {}                               // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime);    // �`�揈��
    void DrawDebug();                         // �f�o�b�O�`��

public:
    Transform* GetTransform() { return model->GetTransform(); }
    void SetMaterialColor(DirectX::XMFLOAT4 color) { materialColor = color; }
    DirectX::XMFLOAT4 GetMaterialColor() { return materialColor; }

    static float renderLengthXLimit_;   // box��`�悷�鋗������

public:
    // Imgui���O���h�~�̂��߂̂�� //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui���O���h�~�̂��߂̂�� //
private:
    std::string name;   // Imgui�p
    std::unique_ptr<Model>model = nullptr;

    DirectX::XMFLOAT4 materialColor = { 1.0f, 1.0f, 1.0f, 1.0f };
};

