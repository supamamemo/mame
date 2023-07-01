#include "../Graphics/Graphics.h"
#include "EffectManager.h"

// 初期化
void EffectManager::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    // Effekseerレンダラ生成
    effekseerRenderer = EffekseerRendererDX11::Renderer::Create(graphics.GetDevice(),
        graphics.GetDeviceContext(), 2048);

    // Effekseerマネージャー生成
    effekseerManager = Effekseer::Manager::Create(2048);

    // Effekseerレンダラの各種設定(特別なカスタマイズをしない場合は定型的に以下の設定でOK)
    effekseerManager->SetSpriteRenderer(effekseerRenderer->CreateSpriteRenderer());
    effekseerManager->SetRibbonRenderer(effekseerRenderer->CreateRibbonRenderer());
    effekseerManager->SetRingRenderer(effekseerRenderer->CreateRingRenderer());
    effekseerManager->SetTrackRenderer(effekseerRenderer->CreateTrackRenderer());
    effekseerManager->SetModelRenderer(effekseerRenderer->CreateModelRenderer());
    // Effekseer内でのローダーの設定(特別なカスタマイズをしない場合は以下の設定でOK)
    effekseerManager->SetTextureLoader(effekseerRenderer->CreateTextureLoader());
    effekseerManager->SetModelLoader(effekseerRenderer->CreateModelLoader());
    effekseerManager->SetMaterialLoader(effekseerRenderer->CreateMaterialLoader());

    // Effekseerを左手亜座標系で計算する
    effekseerManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

// 終了化
void EffectManager::Finalize()
{
    // EffekseerManagerなどはスマートポインタによって破棄されるので何もしない
}

// 更新処理
void EffectManager::Update(float elapsedTime)
{
    // エフェクト更新処理(引数にはフレームの通過時間を渡す)
    effekseerManager->Update(elapsedTime * 60.0f);
}

// 描画処理
void EffectManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    // ビュー&プロジェクション行列をEffekseerレンダラに設定
    effekseerRenderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    effekseerRenderer->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&projection));

    // Effekseer描画開始
    effekseerRenderer->BeginRendering();

    // Effekseer描画実行
    // マネージャー単位で描画するので描画順を制御する場合はマネージャーを複数個制作し、
    // Draw()関数を実行する順序で制御できそう
    effekseerManager->Draw();

    // Effekseer描画終了
    effekseerRenderer->EndRendering();
}

#if 0
#include "EffectManager.h"
#include "Effekseer.h"


void DeviceDX11::SetupEffekseerModules(::Effekseer::ManagerRef efkManager)
{
	// Create a  graphics device
	// 描画デバイスの作成
	auto graphicsDevice = ::EffekseerRendererDX11::CreateGraphicsDevice(GetID3D11Device(), GetID3D11DeviceContext());

	// Create a renderer of effects
	// エフェクトのレンダラーの作成
	efkRenderer = ::EffekseerRendererDX11::Renderer::Create(graphicsDevice, 8000);

	// Sprcify rendering modules
	// 描画モジュールの設定
	efkManager->SetSpriteRenderer(efkRenderer->CreateSpriteRenderer());
	efkManager->SetRibbonRenderer(efkRenderer->CreateRibbonRenderer());
	efkManager->SetRingRenderer(efkRenderer->CreateRingRenderer());
	efkManager->SetTrackRenderer(efkRenderer->CreateTrackRenderer());
	efkManager->SetModelRenderer(efkRenderer->CreateModelRenderer());

	// Specify a texture, model, curve and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	efkManager->SetTextureLoader(efkRenderer->CreateTextureLoader());
	efkManager->SetModelLoader(efkRenderer->CreateModelLoader());
	efkManager->SetMaterialLoader(efkRenderer->CreateMaterialLoader());
	efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	// Specify sound modules
	// サウンドモジュールの設定
	efkSound = ::EffekseerSound::Sound::Create(GetIXAudio2(), 16, 16);

	// Specify a metho to play sound from an instance of efkSound
	// 音再生用インスタンスから再生機能を指定
	efkManager->SetSoundPlayer(efkSound->CreateSoundPlayer());

	// Specify a sound data loader
	// It can be extended by yourself. It is loaded from a file on now.
	// サウンドデータの読込機能を設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	efkManager->SetSoundLoader(efkSound->CreateSoundLoader());
}


::EffekseerRendererDX11::RendererRef efkRenderer;
::EffekseerSound::SoundRef efkSound;
#endif