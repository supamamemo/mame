#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>

#include "../Transform.h"
#include "../../../Source/skinned_mesh.h"

#include "../../Game/Common.h"

class Model
{
public:
    Model(ID3D11Device* device, const char* fbx_filename, bool triangulate = false, float sampling_rate = 0);
    Model(ID3D11Device* device, const char* fbx_filename, std::vector<std::string>& animation_filenames, bool triangulate = false, float sampling_rate = 0);
    ~Model() {}

    Transform* GetTransform() { return &transform; }

    //const skinned_mesh* GetSkinnedMesh()const { return &skinned_meshes; }
     
    
public: // �擾�E�ݒ�֐��֘A
    // �A�j���[�V�����f�[�^�擾
    std::vector<animation>* GetAnimation() { return &skinned_meshes.animation_clips; };

    // ���݂̃A�j���[�V�����ԍ��̎擾�E�ݒ�
    const int GetCurrentAnimationIndex()& { return currentAnimationIndex; }
    void SetCurrentAnimationIndex(const int& animationIndex) { currentAnimationIndex = animationIndex; }

    // �A�j���[�V�����Đ����x�ݒ�i�r���ōĐ����x��ς������Ƃ��ȂǂɁj
    void SetAnimationSpeed(const float& speed) { animationSpeed = speed; }

public: // �A�j���[�V�����֐��֘A
    // �A�j���[�V�����Đ��ݒ�
    // (�A�j���[�V�����ԍ��E���[�v���邩�ǂ����E�A�j���[�V�����Đ����x�E�X���[�Y�؂�ւ����ԁi���x�j)
    void PlayAnimation(
        const int&   index,
        const bool&  loop,
        const float& speed        = 1.0f,
        const float& blendSeconds = 1.0f
    );

    // �u�����h���̌v�Z�X�V����
    void UpdateBlendRate(NO_CONST float& blendRate, const float& elapsedTime);

    // �A�j���[�V�����X�V����
    void UpdateAnimation(const float& elapsedTime);

    // �A�j���[�V�������Đ������ǂ���
    bool IsPlayAnimation() const;


public: // �f�o�b�O�m�F�p
    bool isDebugBlendAnimation = true;     // �A�j���[�V�����u�����h�I���I�t

public:
    skinned_mesh skinned_meshes;
    Transform    transform;

    animation::keyframe keyframe = {};

private:
    float       animationSpeed          = 1.0f;     // �A�j���[�V�����Đ����x
    float       currentAnimationSeconds = 0.0f;     // ���݂̃A�j���[�V�����Đ�����

    float       animationBlendTime      = 0.0f;     // ���݂̃u�����h�^�C�}�[
    float       animationBlendSeconds   = 0.0f;     // �u�����h����

    int         currentAnimationIndex   = -1;	    // ���݂̃A�j���[�V�����ԍ�

    bool        animationLoopFlag       = false;    // �A�j���[�V���������[�v�Đ����邩
    bool        animationEndFlag        = false;    // �A�j���[�V�������I��������

};

