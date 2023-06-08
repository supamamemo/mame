#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>

#include "../Transform.h"
#include "../../../Source/skinned_mesh.h"

class Model
{
public:
    Model(ID3D11Device* device, const char* fbx_filename, bool triangulate = false, float sampling_rate = 0);
    Model(ID3D11Device* device, const char* fbx_filename, std::vector<std::string>& animation_filenames, bool triangulate = false, float sampling_rate = 0);
    ~Model() {}

    Transform* GetTransform() { return &transform; }

    //const skinned_mesh* GetSkinnedMesh()const { return &skinned_meshes; }

public:
    skinned_mesh skinned_meshes;
    Transform transform;
};

