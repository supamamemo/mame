#pragma once

#include <d3d11.h>
#include <wrl.h>

#include <directxmath.h>

#include <vector>
#include <string>

#include <fbxsdk.h>

#include <unordered_map>

// cereal
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/unordered_map.hpp>

#ifdef USE_IMGUI
#include "../External/imgui/imgui.h"
#include "../External/imgui/imgui_internal.h"
#include "../External/imgui/imgui_impl_dx11.h"
#include "../External/imgui/imgui_impl_win32.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//extern ImWchar glyphRangesJapanese[];
#endif

// DirectXMath�\���̗p��serialize�e���v���[�g
namespace DirectX
{
    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT2& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y)
        );
    }

    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT3& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y),
            cereal::make_nvp("z", v.z)
        );
    }

    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT4& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y),
            cereal::make_nvp("z", v.z),
            cereal::make_nvp("w", v.w)
        );
    }

    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT4X4& m)
    {
        archive(
            cereal::make_nvp("_11", m._11), cereal::make_nvp("_12", m._12),
            cereal::make_nvp("_13", m._13), cereal::make_nvp("_14", m._14),
            cereal::make_nvp("_21", m._21), cereal::make_nvp("_22", m._22),
            cereal::make_nvp("_23", m._23), cereal::make_nvp("_24", m._24),
            cereal::make_nvp("_31", m._31), cereal::make_nvp("_32", m._32),
            cereal::make_nvp("_33", m._33), cereal::make_nvp("_34", m._34),
            cereal::make_nvp("_41", m._41), cereal::make_nvp("_42", m._42),
            cereal::make_nvp("_43", m._43), cereal::make_nvp("_44", m._44)
        );
    }
}

// �X�P���g��
struct skeleton
{
    // �{�[��
    struct bone
    {
        uint64_t unique_id{ 0 };
        std::string name;
        // 'parent_index' �́A���ꎩ�̂��܂ޔz����̐e�{�[���̈ʒu���Q�Ƃ���C���f�b�N�X�ł��B
        int64_t parent_index{ -1 }; // -1: �{�[���͌Ǘ����Ă��܂�
        // 'node_index' �̓V�[���̃m�[�h�z����Q�Ƃ���C���f�b�N�X�ł��B
        int64_t node_index{ 0 };

        // 'offset_transform' �́A���f�� (���b�V��) ��Ԃ���{�[�� (�m�[�h) �V�[���ւ̕ϊ��Ɏg�p����܂��B
        DirectX::XMFLOAT4X4 offset_transform{
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };

        bool is_orphan()const { return parent_index < 0; };

        // serialize
        template<class T>
        void serialize(T& archive)
        {
            archive(unique_id, name, parent_index, node_index, offset_transform);
        }
    };
    std::vector<bone>bones;
    int64_t indexof(uint64_t unique_id)const
    {
        int64_t index{ 0 };
        for (const bone& bone : bones)
        {
            if (bone.unique_id == unique_id)
            {
                return index;
            }
            ++index;
        }
        return -1;
    }

    // serialize
    template<class T>
    void serialize(T& archive)
    {
        archive(bones);
    }
};

// �A�j���[�V����
struct animation
{
    std::string name;
    float sampling_rate{ 0 };

    struct keyframe
    {
        struct node
        {
            // 'global_transform' is used to convert from local space of node to global space of scene.
            DirectX::XMFLOAT4X4 global_transform{
                1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1
            };

            // �m�[�h�̕ϊ��f�[�^�ɂ́A�e�ɑ΂���ړ��A��]�A�X�P�[�����O �x�N�g�����܂܂�܂��B
            DirectX::XMFLOAT3 scaling{ 1,1,1 };
            DirectX::XMFLOAT4 rotation{ 0,0,0,1 };  // Rotation quaternion
            DirectX::XMFLOAT3 translation{ 0,0,0 };

            // serialize
            template<class T>
            void serialize(T& archive)
            {
                archive(global_transform, scaling, rotation, translation);
            }
        };
        std::vector<node> nodes;

        // serialize
        template<class T>
        void serialize(T& archive)
        {
            archive(nodes);
        }
    };
    std::vector<keyframe> sequence;

    // serialize
    template<class T>
    void serialize(T& archive)
    {
        archive(name, sampling_rate, sequence);
    }
};

// �V�[��
struct scene
{
    struct node
    {
        uint64_t unique_id{ 0 };
        std::string name;
        FbxNodeAttribute::EType attribute{ FbxNodeAttribute::EType::eUnknown };
        int64_t parent_index{ -1 };

        // serialize
        template<class T>
        void serialize(T& archive)
        {
            archive(unique_id, name, attribute, parent_index);
        }
    };
    std::vector<node> nodes;

    int64_t indexof(uint64_t unique_id)const
    {
        int64_t index{ 0 };
        for (const node& node : nodes)
        {
            if (node.unique_id == unique_id)
            {
                return index;
            }
            ++index;
        }
        return -1;
    }

    // serialize
    template<class T>
    void serialize(T& archive)
    {
        archive(nodes);
    }
};

class skinned_mesh
{
public:
    // ���_���
    static const int MAX_BONE_INFLUENCES{ 4 };
    struct vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT4 tangent;
        DirectX::XMFLOAT2 texcoord;
        float bone_weights[MAX_BONE_INFLUENCES]{ 1,0,0,0 };
        uint32_t bone_indices[MAX_BONE_INFLUENCES]{};

        // serialize
        template<class T>
        void serialize(T& archive)
        {
            archive(position, normal, tangent, texcoord, bone_weights, bone_indices);
        }
    };

    // �萔�o�b�t�@�[
    static const int MAX_BONES{ 256 };
    struct constants
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 material_color;
        DirectX::XMFLOAT4X4 bone_transforms[MAX_BONES]{
            {
                1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1
            }
        };
    };

    // ���b�V��
    struct mesh
    {
        uint64_t unique_id{ 0 };
        std::string name;
        // 'node_index' is an index that refers to the node array of the scene.
        int64_t node_index{ 0 };

        std::vector<vertex> vertices;
        std::vector<uint32_t> indices;

        // �T�u�Z�b�g
        struct subset
        {
            uint64_t material_unique_id{ 0 };
            std::string material_name;

            uint32_t start_index_location{ 0 };
            uint32_t index_count{ 0 };

            // serialize
            template<class T>
            void serialize(T& archive)
            {
                archive(material_unique_id, material_name, start_index_location, index_count);
            }
        };
        std::vector<subset> subsets;

        // default_global_transform �V�[�����̈ʒu�E�p���E�X�P�[�����
        DirectX::XMFLOAT4X4 default_global_transform{
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };

        // �X�P���g��
        skeleton bind_pose;

        // �o�E���f�B���O�{�b�N�X
        DirectX::XMFLOAT3 bounding_box[2]
        {
            { +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX },
            { -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX }
        };

        // serialize
        template<class T>
        void serialize(T& archive)
        {
            archive(unique_id, name, node_index, subsets, default_global_transform,
                bind_pose, bounding_box, vertices, indices);
        }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
        friend class skinned_mesh;
    };
    std::vector<mesh> meshes;

    // �}�e���A��
    struct material
    {
        uint64_t unique_id{ 0 };
        std::string name;

        DirectX::XMFLOAT4 Ka{ 0.2f,0.2f,0.2f,1.0f };
        DirectX::XMFLOAT4 Kd{ 0.8f,0.8f,0.8f,1.0f };
        DirectX::XMFLOAT4 Ks{ 1.0f,1.0f,1.0f,1.0f };

        std::string texture_filenames[4];
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_views[4];

        // serialize
        template<class T>
        void serialize(T& archive)
        {
            archive(unique_id, name, Ka, Kd, Ks, texture_filenames);
        }
    };
    std::unordered_map<uint64_t, material> materials;
    // unordered_map�́A����L�[�̗v�f�𕡐��i�[�ł����A�i�[�����K�肳��Ă��Ȃ��R���e�i

    // �A�j���[�V����
    std::vector<animation> animation_clips;

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;   // ���_�V�F�[�_�[
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;     // �s�N�Z���V�F�[�_�[
    Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;     // ���̓��C�A�E�g
    Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;


    // Instancing
    struct Instance
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 color;
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> InstancingConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> InstancingBuffer;


    void create_com_objects(ID3D11Device* device, const char* fbx_filename);

public:
    skinned_mesh(ID3D11Device* device, const char* fbx_filename, bool triangulate = false, float sampling_rate = 0);
    skinned_mesh(ID3D11Device* device, const char* fbx_filename, std::vector<std::string>& animation_filenames, bool triangulate = false, float sampling_rate = 0);
    virtual ~skinned_mesh() = default;



    void render(ID3D11DeviceContext* immeddiate_context,
        const DirectX::XMFLOAT4X4 world, const DirectX::XMFLOAT4& material_color,
        const animation::keyframe* keyframe,
        ID3D11PixelShader* alternative_pixel_shader = nullptr);

    void update_animation(animation::keyframe& keyframe);

    bool append_animations(const char* animation_filename, float sampling_rate);
    void blend_animations(const animation::keyframe* keyframes[2], float factor, animation::keyframe& keyframe);

    void Drawdebug();

    D3D11_TEXTURE2D_DESC mask_texture2dDesc;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mask_texture[5];
    int mask_texture_value = 0;
    struct dissolve_constants
    {
        DirectX::XMFLOAT4 parameters;   // x:�f�B�]���u�K���ʁAyzw:��
    };
    float dissolve_value{ 0.0f };
    float dissolve_value1{ 0.0f };
    Microsoft::WRL::ComPtr<ID3D11Buffer> dissolve_constant_buffer;





protected:
    scene scene_view;
    // ���b�V��
    void fetch_meshes(FbxScene* fbx_scene, std::vector<mesh>& meshes);
    // �}�e���A��
    void fetch_materials(FbxScene* fbx_scene, std::unordered_map<uint64_t, material>& materials);
    // �X�P���g��
    void fetch_skeleton(FbxMesh* fbx_mesh, skeleton& bind_pose);
    // �A�j���[�V����
    void fetch_animations(FbxScene* fbx_scene, std::vector<animation>& animation_clips, float sampling_rate/*If this value is 0, the animation data will be sampled at the default frame rate.*/);
    // �V�[��
    void fetch_scene(const char* fbx_filename, bool triangulate, float sampling_rate/*If this value is 0, the animation data will be sampled at the default frame rate.*/);

public:
    // BOUNDING_BOX
    DirectX::XMFLOAT3 boundingBox[2]
    {
        { +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX },
        { -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX }
    };


private:
    // BOUNDING_BOX
    void compute_bounding_box()
    {
        // Calculate a bounding box surrounding all meshes. In addition, coordinate values of all bounding boxes are converted to global space.
        for (mesh& mesh : meshes)
        {
            DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&mesh.default_global_transform);
            DirectX::XMStoreFloat3(&mesh.bounding_box[0], DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&mesh.bounding_box[0]), M));
            DirectX::XMStoreFloat3(&mesh.bounding_box[1], DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&mesh.bounding_box[1]), M));

            if (mesh.bounding_box[0].x > mesh.bounding_box[1].x) std::swap<float>(mesh.bounding_box[0].x, mesh.bounding_box[1].x);
            if (mesh.bounding_box[0].y > mesh.bounding_box[1].y) std::swap<float>(mesh.bounding_box[0].y, mesh.bounding_box[1].y);
            if (mesh.bounding_box[0].z > mesh.bounding_box[1].z) std::swap<float>(mesh.bounding_box[0].z, mesh.bounding_box[1].z);

            boundingBox[0].x = std::min<float>(mesh.bounding_box[0].x, boundingBox[0].x);
            boundingBox[0].y = std::min<float>(mesh.bounding_box[0].y, boundingBox[0].y);
            boundingBox[0].z = std::min<float>(mesh.bounding_box[0].z, boundingBox[0].z);
            boundingBox[1].x = std::max<float>(mesh.bounding_box[1].x, boundingBox[1].x);
            boundingBox[1].y = std::max<float>(mesh.bounding_box[1].y, boundingBox[1].y);
            boundingBox[1].z = std::max<float>(mesh.bounding_box[1].z, boundingBox[1].z);
        }
    }
};

