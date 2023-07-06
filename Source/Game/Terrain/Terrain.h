#pragma once

#include <memory>
#include "../../Mame/Graphics/Model.h"
#include "../../GeometricPrimitive.h"
#include "../Collision.h"

class Terrain
{
public:
    // �n�`�̎��
    enum class Type
    {
        None    = -1,
        Normal,         // ���ʂ̒n�`
        Move,           // �����n�`
    };

public:
    Terrain(const char* const filename);
    virtual ~Terrain() = default;

    virtual void Initialize();                           // ������
    virtual void Finalize()                       {}     // �I����
    virtual void Update(const float& elapsedTime) {}     // �X�V����
    virtual void Render(const float& elapsedTime);       // �`�揈��
    virtual void DrawDebug();                            // �f�o�b�O�`��

    virtual void OnRiding() {}                           // ���ꂽ���ɌĂ΂��

    void Destroy();                                      // ��������������

public:
    void UpdateAABB();                                   // AABB�X�V����

public:
    Transform* GetTransform() const { return model_->GetTransform(); }   // �g�����X�t�H�[���擾

    // Imgui���O���h�~�̂��߂̂�� //
    const char* const GetName() const { return name.c_str(); }
    void SetName(const char* const n) { name = n; }

    const DirectX::XMFLOAT4& GetMaterialColor() const { return materialColor; }
    void SetMaterialColor(const DirectX::XMFLOAT4& color) { materialColor = color; }
public:
    std::unique_ptr<Model> model_;                         // ���f��

    std::unique_ptr<GeometricPrimitive> geometricAABB_;   // �����蔻��`��p�W�I���g���b�N�v���~�e�B�u
    Collision::AABB aabb_ = {};                           // �����蔻��\����

    Terrain::Type terrainType_ = Terrain::Type::None;

public:
    static float    renderLengthXLimit_;     // �`�悳�����E����
    static int      nameNum;

protected:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders = {};

    DirectX::XMFLOAT4   debugMaterialColor  = { 1.0f, 0.0f, 0.0f, 0.4f }; // �}�e���A���J���[(debug�p)
    DirectX::XMFLOAT4   materialColor       = { 1.0f, 1.0f, 1.0f, 1.0f };

    DirectX::XMFLOAT3   velocity_           = {};
    DirectX::XMFLOAT3   originPosition_     = {};

private:
    // Imgui���O���h�~�̂��߂̂�� //    
    std::string name = {};   // Imgui�p

    DirectX::XMFLOAT3 defaultMin_ = {};
    DirectX::XMFLOAT3 defaultMax_ = {};

};