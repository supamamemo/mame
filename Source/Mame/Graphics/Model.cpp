#include "Model.h"
#include "ResourceManager.h"

// TODO: ���\�[�X�}�l�[�W���[�̎g�p
#define USE_RESOURCE_MANAGER

Model::Model(ID3D11Device* device, const char* fbx_filename, bool triangulate, float sampling_rate)
{   
#ifdef USE_RESOURCE_MANAGER // ���\�[�X�}�l�[�W���[����
    skinned_meshes = ResourceManager::Instance().LoadModelResource(
        device, 
        fbx_filename, 
        triangulate, 
        sampling_rate
    );
#else // ���\�[�X�}�l�[�W���[�Ȃ�
    skinned_meshes = std::make_unique<skinned_mesh>(device, fbx_filename, triangulate, sampling_rate);
#endif

}

Model::Model(ID3D11Device* device, const char* fbx_filename, std::vector<std::string>& animation_filenames, bool triangulate, float sampling_rate)
{
    skinned_meshes = ResourceManager::Instance().LoadModelResource(
        device, fbx_filename, 
        animation_filenames, 
        triangulate, 
        sampling_rate
    );
}


void Model::PlayAnimation(
    const int& index, const bool& loop,
    const float& speed, const float& blendSeconds)
{
    // �ݒ�p�̃A�j���[�V�����ԍ������݂̃A�j���[�V�����ԍ��Ɠ����ꍇ��return
    if (currentAnimationIndex == index) return;

    currentAnimationIndex   = index;    // �Đ�����A�j���[�V�����ԍ���ݒ�
    currentAnimationSeconds = 0.0f;     // �A�j���[�V�����Đ����ԃ��Z�b�g

    animationLoopFlag       = loop;     // ���[�v�����邩
    animationEndFlag        = false;    // �Đ��I���t���O�����Z�b�g

    animationSpeed          = speed;    // �A�j���[�V�����Đ����x

    animationBlendTime      = 0.0f;
    animationBlendSeconds   = blendSeconds;
}


void Model::UpdateBlendRate(NO_CONST float& blendRate, const float& elapsedTime)
{
    if (animationBlendTime < animationBlendSeconds)
    {
        animationBlendTime += elapsedTime;
        if (animationBlendTime >= animationBlendSeconds)
        {
            animationBlendTime = animationBlendSeconds;
        }
        blendRate = animationBlendTime / animationBlendSeconds;
        blendRate *= blendRate;
    }
}


void Model::UpdateAnimation(const float& elapsedTime)
{
    // �Đ����łȂ��Ȃ珈�����Ȃ�
    if (!IsPlayAnimation()) return;

    // �ŏI�t���[������(�Đ��I���t���O�������Ă���΍Đ��I��)
    if (animationEndFlag)
    {
        animationEndFlag = false; // �I���t���O�����Z�b�g
        currentAnimationIndex = -1;    // �A�j���[�V�����ԍ����Z�b�g
        return;
    }

    // �A�j���[�V�����Đ����Ԍo��
    currentAnimationSeconds += elapsedTime;

    // �w��̃A�j���[�V�����f�[�^���擾
    NO_CONST animation& animation = GetAnimation()->at(currentAnimationIndex);

    // ���݂̃t���[�����擾
    const float  frameIndex_float = (currentAnimationSeconds * animation.sampling_rate) * animationSpeed; // �x��������܂Ȃ̂ňꎞ�I��float�ϐ��Ɋi�[
    const size_t frameIndex = static_cast<const size_t>(frameIndex_float);

    // �Ō�̃t���[�����擾
    const size_t frameEnd = (animation.sequence.size() - 1);


    // �A�j���[�V�������Đ��������Ă����ꍇ
    if (frameIndex > frameEnd)
    {
        // ���[�v�t���O�������Ă���΍Đ����Ԃ������߂�
        if (animationLoopFlag)
        {
            currentAnimationSeconds = 0.0f;
            return;
        }
        // ���[�v�Ȃ��Ȃ�Đ��I���t���O�𗧂Ă�
        else
        {
            animationEndFlag = true;
            return;
        }
    }
    // �L�[�t���[�����X�V����Ă��ăA�j���[�V�������Đ��������Ă��Ȃ��Ƃ��̓A�j���[�V�������X���[�Y�ɐ؂�ւ���
    else if (isDebugBlendAnimation && (keyframe.nodes.size() > 0) && frameIndex < frameEnd)
    {
        // �u�����h���̌v�Z
        NO_CONST float blendRate = 1.0f;
        UpdateBlendRate(blendRate, elapsedTime);

        // �L�[�t���[���擾
        const std::vector<animation::keyframe>& keyframes = animation.sequence;

        // ���݂̑O��̃L�[�t���[�����擾
        const animation::keyframe* keyframeArr[2] = {
            &keyframe,
            &keyframes.at(frameIndex + 1)
        };

        // �A�j���[�V���������炩�ɐ؂�ւ���
        skinned_meshes->blend_animations(keyframeArr, blendRate, keyframe);

        // �A�j���[�V�����g�����X�t�H�[���X�V
        skinned_meshes->update_animation(keyframe);
    }
    // �L�[�t���[������x���X�V����Ă��Ȃ��ăA�j���[�V�������Đ��������Ă��Ȃ���Ό��݂̃t���[����ۑ�
    else
    {
        keyframe = animation.sequence.at(frameIndex);
    }
}


bool Model::IsPlayAnimation() const
{
    if (currentAnimationIndex < 0) return false;

    const int animationIndexEnd = static_cast<int>(skinned_meshes->animation_clips.size());
    if (currentAnimationIndex >= animationIndexEnd) return false;

    return true;
}
