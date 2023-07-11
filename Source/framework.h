#pragma once
//#pragma comment (lib,"External/Effekseer162a/lib/vs2019/MT/x64/Debug/Effekseer.lib")

#include <d3d11.h>

#include <windows.h>
#include <tchar.h>
#include <sstream>

#include <wrl.h>

#include "misc.h"
#include "high_resolution_timer.h"

#include "./Mame/Graphics/Graphics.h"
#include "./Mame/Input/Input.h"


#include "shader.h"
#include "sprite.h"
#include "sprite_batch.h"
#include "GeometricPrimitive.h"
#include "static_mesh.h"
#include "skinned_mesh.h"
#include "framebuffer.h"
#include "fullscreen_quad.h"

#include "gltf_model.h"

#include "./Mame/Audio.h"

#ifdef USE_IMGUI
#include "../External/imgui/imgui.h"
#include "../External/imgui/imgui_internal.h"
#include "../External/imgui/imgui_impl_dx11.h"
#include "../External/imgui/imgui_impl_win32.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern ImWchar glyphRangesJapanese[];
#endif

CONST LONG SCREEN_WIDTH{ 1280 };
CONST LONG SCREEN_HEIGHT{ 720 };
//CONST LONG SCREEN_WIDTH{ 1920 };
//CONST LONG SCREEN_HEIGHT{ 1080 };
CONST BOOL FULLSCREEN{ FALSE };
CONST LPCWSTR APPLICATION_NAME{ L"MAMEO" };



class framework
{
private:
	Graphics graphics;
	Input input;

public:

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_states[4]; // ID3D11DepthStencilStateインターフェイス
	// 深度ステンシル状態インターフェイスは、
	// 出力マージャーステージにバインドできる深度ステンシル状態の説明を保持します
	
	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_states[4]; // ID3D11BlendStateインターフェイス
	// blend-stateインターフェイスは、output-mergerステージにバインドできるブレンド状態の説明を保持します

	std::unique_ptr<Sprite> sprites[8];	// sprite*型配列

	std::unique_ptr<sprite_batch> sprite_batches[8];

	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_states[3];

	struct scene_constants
	{
		DirectX::XMFLOAT4X4 view_projection;	// ビュー・プロジェクション変換行列
		DirectX::XMFLOAT4 light_direction;		// ライトの向き
		DirectX::XMFLOAT4 camera_position;		// カメラ位置
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffers[8];

	std::unique_ptr<GeometricPrimitive> geometric_primitives[8];

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>rasterizer_states[4];
	
	std::unique_ptr<static_mesh> static_meshes[8];

	std::unique_ptr<skinned_mesh> skinned_meshes[8];

	std::unique_ptr<framebuffer> framebuffers[8];

	std::unique_ptr<fullscreen_quad> bit_block_transfer;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[8];

	struct parametric_constants
	{
		float extraction_threshold{ 0.8f };
		float gaussian_sigma{ 1.0f };
		float bloom_intensity{ 1.0f };
		float exposure{ 1.0f };
	};

	// gltf
	std::unique_ptr<gltf_model> gltf_models[8];

	CONST HWND hwnd;

	framework(HWND hwnd);
	~framework();

	framework(const framework&) = delete;
	framework& operator=(const framework&) = delete;
	framework(framework&&) noexcept = delete;
	framework& operator=(framework&&) noexcept = delete;




	int run()
	{
		MSG msg{};

		if (!initialize())
		{
			return 0;
		}

#ifdef USE_IMGUI
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 14.0f, nullptr, glyphRangesJapanese);
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(graphics.GetDevice(), graphics.GetDeviceContext());
		ImGui::StyleColorsDark();
#endif

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				tictoc.tick();
				calculate_frame_stats();
				update(tictoc.time_interval());
				render(tictoc.time_interval());
			}
		}

#ifdef USE_IMGUI
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
#endif

#if 1
		BOOL fullscreen = 0;
		graphics.GetSwapChain()->GetFullscreenState(&fullscreen, 0);
		if (fullscreen)
		{
			graphics.GetSwapChain()->SetFullscreenState(FALSE, 0);
		}
#endif

		return uninitialize() ? static_cast<int>(msg.wParam) : 0;
	}

	LRESULT CALLBACK handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
#ifdef USE_IMGUI
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) { return true; }
#endif
		switch (msg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps{};
			BeginPaint(hwnd, &ps);

			EndPaint(hwnd, &ps);
		}
		break;

		case WM_DESTROY:
			
			PostQuitMessage(0);
			break;
		case WM_CREATE:
			break;
		case WM_KEYDOWN:
			if (wparam == VK_ESCAPE)
			{
				//PostMessage(hwnd, WM_CLOSE, 0, 0);
			}
			break;
		case WM_ENTERSIZEMOVE:
			tictoc.stop();
			break;
		case WM_EXITSIZEMOVE:
			tictoc.start();
			break;
		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

private:
	bool initialize();
	void update(float elapsed_time/*Elapsed seconds from last frame*/);
	void render(float elapsed_time/*Elapsed seconds from last frame*/);
	bool uninitialize();

private:
	high_resolution_timer tictoc;
	uint32_t frames{ 0 };
	float elapsed_time{ 0.0f };
	void calculate_frame_stats()
	{
		if (++frames, (tictoc.time_stamp() - elapsed_time) >= 1.0f)
		{
			float fps = static_cast<float>(frames);
			std::wostringstream outs;
			outs.precision(6);
			outs << APPLICATION_NAME << L" : FPS : " << fps << L" / " << L"Frame Time : " << 1000.0f / fps << L" (ms)";
			SetWindowTextW(hwnd, outs.str().c_str());

			frames = 0;
			elapsed_time += 1.0f;
		}
	}

	struct Imgui_debug
	{
		int imgui_state = 0;
		float imgui_color = 0.5f;
		float dx = 100, dy = 100;		// 短形の左上の座標(スクリーン座標系)
		float dw = 100, dh = 100;		// 短形のサイズ(スクリーン座標系)
		float r = 1, g = 1, b = 1, a = 1;	// 色
		float angle = 0.0f;		// 角度
		int state = 0;

		// geometricprimitive
		DirectX::XMFLOAT3 geoAngle = {};
		DirectX::XMFLOAT3 geoScale = { 1,1,1 };
		DirectX::XMFLOAT3 geoPosition = { 0,0,0 };
		DirectX::XMFLOAT4 color = { 0.3f,0.3f,0.3f,1.0f };
		int rasterizer_state = 0;
		float scaleFactor = 1;

		// model
		float modelAngle = 0;
		float modelLenght = 0;
		float modelBlending[2] = {};

		// camera
		DirectX::XMFLOAT4 eye = { 0.0f,0.0f,-10.0f,1.0f };
		DirectX::XMFLOAT4 focus = { 0.0f,0.0f,0.0f,1.0f };
		DirectX::XMFLOAT4 up = { 0.0f,1.0f,0.0f,0.0f };

		DirectX::XMFLOAT4 right = { 0,0,1,0 };
		DirectX::XMFLOAT4 camera = { 0,0,1,0 };

		// hlsl
		parametric_constants parametric_constants;

		void DrawDebug()
		{
			if (ImGui::TreeNode("imgui"))
			{
				ImGui::SliderInt("state", &imgui_state, 0, 4);
				ImGui::SliderFloat("alpha", &imgui_color, 0.1f, 1.0f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("2d"))
			{
				ImGui::SliderFloat("dx", &dx, -1000.0f, 1280.0f);
				ImGui::SliderFloat("dy", &dy, -1000.0f, 720.0f);
				ImGui::SliderFloat("dw", &dw, 0.0f, 6000.0f);
				ImGui::SliderFloat("dh", &dh, 0.0f, 6000.0f);
				ImGui::SliderFloat("r", &r, 0.0f, 1.0f);
				ImGui::SliderFloat("g", &g, 0.0f, 1.0f);
				ImGui::SliderFloat("b", &b, 0.0f, 1.0f);
				ImGui::SliderFloat("a", &a, 0.0f, 1.0f);
				ImGui::SliderFloat("angle", &angle, 0.0f, 360.0f);
				ImGui::SliderInt("state", &state, 0, 2);
				ImGui::ColorPicker4("COLOR", &r);
				ImGui::TreePop();
			}
			
			// geometricPrimitive
			if (ImGui::TreeNode("Geoprimitive"))
			{
				ImGui::SliderFloat3("Position", &geoPosition.x,-20,20);
				ImGui::SliderFloat3("Scale", &geoScale.x, 0, 10);
				ImGui::SliderFloat("scaleFactor", &scaleFactor,0.0f,100.0f);
				geoScale.x = scaleFactor;
				geoScale.y = scaleFactor;
				geoScale.z = scaleFactor;

				DirectX::XMFLOAT3 a;
				a.x = DirectX::XMConvertToDegrees(geoAngle.x);
				a.y = DirectX::XMConvertToDegrees(geoAngle.y);
				a.z = DirectX::XMConvertToDegrees(geoAngle.z);
				ImGui::SliderFloat3("Angle", &a.x, -360.0f, 360.0f);
				geoAngle.x = DirectX::XMConvertToRadians(a.x);
				geoAngle.y = DirectX::XMConvertToRadians(a.y);
				geoAngle.z = DirectX::XMConvertToRadians(a.z);

				ImGui::ColorPicker4("COLOR", &color.x);

				ImGui::SliderInt("rasterize_state", &rasterizer_state, 0, 2);

				ImGui::TreePop();

				if (ImGui::TreeNode("Model"))
				{
					float modelA;
					modelA = DirectX::XMConvertToDegrees(modelAngle);
					ImGui::SliderFloat("angle", &modelA, -360.0f, 360.0f);
					modelAngle = DirectX::XMConvertToRadians(modelA);
					ImGui::SliderFloat("lenght", &modelLenght, 0.0f, 400.0f);
					ImGui::SliderFloat2("blending", &modelBlending[0], 0.0f, 80.0f);
					ImGui::TreePop();
				}
			}

			// camera
			if (ImGui::TreeNode("Camera"))
			{
				ImGui::SliderFloat4("eye", &eye.x, -10.0f, 10.0f);
				ImGui::SliderFloat4("focus", &focus.x, -10.0f, 10.0f);
				ImGui::SliderFloat4("up", &up.x, -10.0f, 10.0f);

				ImGui::SliderFloat4("position", &camera.x, 10.0f, 10.0f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("right"))
			{
				ImGui::SliderFloat4("right", &right.x, -1.0f, 1.0f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("hlsl"))
			{
				ImGui::SliderFloat("extraction_thershold", &parametric_constants.extraction_threshold, 0.0f, 5.0f);
				ImGui::SliderFloat("gaussian_sigma", &parametric_constants.gaussian_sigma, +0.0f, +10.0f);
				ImGui::SliderFloat("bloom_intensity", &parametric_constants.bloom_intensity, +0.0f, +10.0f);
				ImGui::SliderFloat("exposure", &parametric_constants.exposure, +0.0f, +10.0f);
				ImGui::TreePop();
			}

		}
	}imgui_debug;
};