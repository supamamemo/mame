#pragma once

#include <memory>
#include "../../Mame/Graphics/Model.h"
#include "../../GeometricPrimitive.h"
#include "../Collision.h"

class Terrain
{
public:
    Terrain(const char* const filename);
    virtual ~Terrain() = default;

    virtual void Initialize()                       = 0; // ������
    virtual void Finalize()                         = 0; // �I����
    virtual void Begin()                            = 0; // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(const float& elapsedTime)   = 0; // �X�V����
    virtual void End()                              = 0; // ���t���[����ԍŌ�ɌĂ΂��
    virtual void Render(const float& elapsedTime);       // �`�揈��
    virtual void DrawDebug();                            // �f�o�b�O�`��

public:
    void UpdateAABB();                                   // AABB�X�V����

public:
    Transform* GetTransform() const { return model->GetTransform(); }   // �g�����X�t�H�[���擾

public:
    std::unique_ptr<Model> model;                         // ���f��

    std::unique_ptr<GeometricPrimitive> geometricAABB_;   // �����蔻��`��p�W�I���g���b�N�v���~�e�B�u
    Collision::AABB aabb_ = {};                           // �����蔻��\����


    // Imgui���O���h�~�̂��߂̂�� //
    const char* const GetName() const { return name.c_str(); }
    void SetName(const char* const n) { name = n; }

public:
    static int nameNum;

protected:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders = {};

    DirectX::XMFLOAT4 materialColor = { 0.0f, 1.0f, 0.0f, 0.4f }; // �}�e���A���J���[

private:
    // Imgui���O���h�~�̂��߂̂�� //    
    std::string name;   // Imgui�p

    DirectX::XMFLOAT3 defaultMin_ = {};
    DirectX::XMFLOAT3 defaultMax_ = {};
};