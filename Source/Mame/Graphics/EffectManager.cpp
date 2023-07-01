#include "../Graphics/Graphics.h"
#include "EffectManager.h"

// ������
void EffectManager::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    // Effekseer�����_������
    effekseerRenderer = EffekseerRendererDX11::Renderer::Create(graphics.GetDevice(),
        graphics.GetDeviceContext(), 2048);

    // Effekseer�}�l�[�W���[����
    effekseerManager = Effekseer::Manager::Create(2048);

    // Effekseer�����_���̊e��ݒ�(���ʂȃJ�X�^�}�C�Y�����Ȃ��ꍇ�͒�^�I�Ɉȉ��̐ݒ��OK)
    effekseerManager->SetSpriteRenderer(effekseerRenderer->CreateSpriteRenderer());
    effekseerManager->SetRibbonRenderer(effekseerRenderer->CreateRibbonRenderer());
    effekseerManager->SetRingRenderer(effekseerRenderer->CreateRingRenderer());
    effekseerManager->SetTrackRenderer(effekseerRenderer->CreateTrackRenderer());
    effekseerManager->SetModelRenderer(effekseerRenderer->CreateModelRenderer());
    // Effekseer���ł̃��[�_�[�̐ݒ�(���ʂȃJ�X�^�}�C�Y�����Ȃ��ꍇ�͈ȉ��̐ݒ��OK)
    effekseerManager->SetTextureLoader(effekseerRenderer->CreateTextureLoader());
    effekseerManager->SetModelLoader(effekseerRenderer->CreateModelLoader());
    effekseerManager->SetMaterialLoader(effekseerRenderer->CreateMaterialLoader());

    // Effekseer�����舟���W�n�Ōv�Z����
    effekseerManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

// �I����
void EffectManager::Finalize()
{
    // EffekseerManager�Ȃǂ̓X�}�[�g�|�C���^�ɂ���Ĕj�������̂ŉ������Ȃ�
}

// �X�V����
void EffectManager::Update(float elapsedTime)
{
    // �G�t�F�N�g�X�V����(�����ɂ̓t���[���̒ʉߎ��Ԃ�n��)
    effekseerManager->Update(elapsedTime * 60.0f);
}

// �`�揈��
void EffectManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    // �r���[&�v���W�F�N�V�����s���Effekseer�����_���ɐݒ�
    effekseerRenderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    effekseerRenderer->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&projection));

    // Effekseer�`��J�n
    effekseerRenderer->BeginRendering();

    // Effekseer�`����s
    // �}�l�[�W���[�P�ʂŕ`�悷��̂ŕ`�揇�𐧌䂷��ꍇ�̓}�l�[�W���[�𕡐����삵�A
    // Draw()�֐������s���鏇���Ő���ł�����
    effekseerManager->Draw();

    // Effekseer�`��I��
    effekseerRenderer->EndRendering();
}

#if 0
#include "EffectManager.h"
#include "Effekseer.h"


void DeviceDX11::SetupEffekseerModules(::Effekseer::ManagerRef efkManager)
{
	// Create a  graphics device
	// �`��f�o�C�X�̍쐬
	auto graphicsDevice = ::EffekseerRendererDX11::CreateGraphicsDevice(GetID3D11Device(), GetID3D11DeviceContext());

	// Create a renderer of effects
	// �G�t�F�N�g�̃����_���[�̍쐬
	efkRenderer = ::EffekseerRendererDX11::Renderer::Create(graphicsDevice, 8000);

	// Sprcify rendering modules
	// �`�惂�W���[���̐ݒ�
	efkManager->SetSpriteRenderer(efkRenderer->CreateSpriteRenderer());
	efkManager->SetRibbonRenderer(efkRenderer->CreateRibbonRenderer());
	efkManager->SetRingRenderer(efkRenderer->CreateRingRenderer());
	efkManager->SetTrackRenderer(efkRenderer->CreateTrackRenderer());
	efkManager->SetModelRenderer(efkRenderer->CreateModelRenderer());

	// Specify a texture, model, curve and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	efkManager->SetTextureLoader(efkRenderer->CreateTextureLoader());
	efkManager->SetModelLoader(efkRenderer->CreateModelLoader());
	efkManager->SetMaterialLoader(efkRenderer->CreateMaterialLoader());
	efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	// Specify sound modules
	// �T�E���h���W���[���̐ݒ�
	efkSound = ::EffekseerSound::Sound::Create(GetIXAudio2(), 16, 16);

	// Specify a metho to play sound from an instance of efkSound
	// ���Đ��p�C���X�^���X����Đ��@�\���w��
	efkManager->SetSoundPlayer(efkSound->CreateSoundPlayer());

	// Specify a sound data loader
	// It can be extended by yourself. It is loaded from a file on now.
	// �T�E���h�f�[�^�̓Ǎ��@�\��ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	efkManager->SetSoundLoader(efkSound->CreateSoundLoader());
}


::EffekseerRendererDX11::RendererRef efkRenderer;
::EffekseerSound::SoundRef efkSound;
#endif