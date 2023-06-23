#include "Block.h"
#include "../Mame/Graphics/Graphics.h"

Block::Block()
{
    Graphics& graphics = Graphics::Instance();
    debugModel = std::make_unique<Model>(graphics.GetDevice(), "./resources/cube.fbx", true);
}

void Block::DrawDebug()
{
    model->GetTransform()->DrawDebug();
}

DirectX::XMFLOAT4X4 Block::SetDebugModelTransform(DirectX::XMFLOAT4X4 transform)
{
    // 0: Target model
    // 1: Bounding box model
    DirectX::XMFLOAT3 dimensions[] = {
#if 1
            {
                model->skinned_meshes.boundingBox[1].x - model->skinned_meshes.boundingBox[0].x,
                model->skinned_meshes.boundingBox[1].y - model->skinned_meshes.boundingBox[0].y,
                model->skinned_meshes.boundingBox[1].z - model->skinned_meshes.boundingBox[0].z,

            },
#else
            { 100.0f, 150.0f, 60.0f },
#endif
            {
                debugModel->skinned_meshes.boundingBox[1].x - debugModel->skinned_meshes.boundingBox[0].x,
                debugModel->skinned_meshes.boundingBox[1].y - debugModel->skinned_meshes.boundingBox[0].y,
                debugModel->skinned_meshes.boundingBox[1].z - debugModel->skinned_meshes.boundingBox[0].z,
            },
        };
    DirectX::XMFLOAT3 centers[] = {
        {
            model->skinned_meshes.boundingBox[0].x + (model->skinned_meshes.boundingBox[1].x - model->skinned_meshes.boundingBox[0].x) * 0.5f,
            model->skinned_meshes.boundingBox[0].y + (model->skinned_meshes.boundingBox[1].y - model->skinned_meshes.boundingBox[0].y) * 0.5f,
            model->skinned_meshes.boundingBox[0].z + (model->skinned_meshes.boundingBox[1].z - model->skinned_meshes.boundingBox[0].z) * 0.5f,
        },
        {
            debugModel->skinned_meshes.boundingBox[0].x + (debugModel->skinned_meshes.boundingBox[1].x - debugModel->skinned_meshes.boundingBox[0].x) * 0.5f,
            debugModel->skinned_meshes.boundingBox[0].y + (debugModel->skinned_meshes.boundingBox[1].y - debugModel->skinned_meshes.boundingBox[0].y) * 0.5f,
            debugModel->skinned_meshes.boundingBox[0].z + (debugModel->skinned_meshes.boundingBox[1].z - debugModel->skinned_meshes.boundingBox[0].z) * 0.5f,
        },
    };

    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(dimensions[0].x / dimensions[1].x, dimensions[0].y / dimensions[1].y, dimensions[0].z / dimensions[1].z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(centers[0].x - centers[1].x, centers[0].y - centers[1].y, centers[0].z - centers[1].z);

    DirectX::XMFLOAT4X4 world; // World transform matrix of bounding box model
    DirectX::XMStoreFloat4x4(&world, S * T * DirectX::XMLoadFloat4x4(&transform/*World transform matrix of target model*/));

    return world;
    }