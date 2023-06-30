#include "ResourceManager.h"

const std::shared_ptr<skinned_mesh> ResourceManager::LoadModelResource(
    NO_CONST ID3D11Device*  device, 
    const char* const       fbx_filename, 
    const bool              triangulate,
    const float             sampling_rate)
{
    // ���f��������
    const ModelMap::iterator it = models.find(fbx_filename);   
    if (it != models.end())
    {
        // �����N(����)���؂�Ă��Ȃ����m�F
        if (it->second.expired() == false)
        {
            // �����������f���Ɠ������f�������������炻���Ԃ�
            return it->second.lock();
        }
    }

    // ������Ȃ�������V�K���f�����\�[�X���쐬�E�ǂݍ���
    const std::shared_ptr<skinned_mesh> model = {
        std::make_shared<skinned_mesh>(
            device, 
            fbx_filename, 
            triangulate, 
            sampling_rate)
    };

    // �}�b�v�ɓo�^
    models[fbx_filename] = model;

    return model;
}

const std::shared_ptr<skinned_mesh> ResourceManager::LoadModelResource(
    NO_CONST ID3D11Device*             device, 
    const char* const                  fbx_filename, 
    NO_CONST std::vector<std::string>& animation_filenames, 
    const bool                         triangulate, 
    const float                        sampling_rate)
{
    // ���f��������
    const ModelMap::iterator it = models.find(fbx_filename);
    if (it != models.end())
    {
        // �����N(����)���؂�Ă��Ȃ����m�F
        if (it->second.expired() == false)
        {
            // �����������f���Ɠ������f�������������炻���Ԃ�
            return it->second.lock();
        }
    }

    // ������Ȃ�������V�K���f�����\�[�X���쐬�E�ǂݍ���
    const std::shared_ptr<skinned_mesh> model = {
        std::make_shared<skinned_mesh>(
            device,
            fbx_filename,
            animation_filenames,
            triangulate,
            sampling_rate)
    };

    // �}�b�v�ɓo�^
    models[fbx_filename] = model;

    return model;
}
