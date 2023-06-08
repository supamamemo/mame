#include "Model.h"

Model::Model(ID3D11Device* device, const char* fbx_filename, bool triangulate, float sampling_rate)
    :skinned_meshes(device, fbx_filename, triangulate, sampling_rate)
{
}

Model::Model(ID3D11Device* device, const char* fbx_filename, std::vector<std::string>& animation_filenames, bool triangulate, float sampling_rate)
    : skinned_meshes(device, fbx_filename, animation_filenames, triangulate, sampling_rate)
{
}
