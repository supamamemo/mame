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
#include "geometric_primitive.h"
#include "static_mesh.h"
#include "skinned_mesh.h"
#include "framebuffer.h"
#include "fullscreen_quad.h"

#include "gltf_model.h"

#ifdef USE_IMGUI
//#include "../../imgui/imgui.h"
//#include "../../imgui/imgui_internal.h"
//#include "../../imgui/imgui_impl_dx11.h"
//#include "../../imgui/imgui_impl_win32.h"
#include "../External/imgui/imgui.h"
#include "../External/imgui/imgui_internal.h"
#include "../External/imgui/imgui_impl_dx11.h"
#include "../External/imgui/imgui_impl_win32.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern ImWchar glyphRangesJapanese[];
#endif

CONST LONG SCREEN_WIDTH{ 1280 };
CONST LONG SCREEN_HEIGHT{ 720 };
CONST BOOL FULLSCREEN{ FALSE };
CONST LPCWSTR APPLICATION_NAME{ L"SUPER MAMEMO" };



class framework
{
private:
	Graphics graphics;
	Input input;

public:
	// 追加メンバ
#if 0
	Microsoft::WRL::ComPtr<ID3D11Device> device;	// デバイスインターフェイス
	// デバイスインターフェイスは仮想アダプタを表します。リソースの作成に使用されます
	// DirectX11で利用する様々なリソースを作成するやつ

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context;	// DeviceContextインターフェイス
	// レンダリングコマンドを生成するデバイスコンテキストを表します
	// 様々な描画命令をGPUに伝えるやつ
	// <GPU>コンピュータゲームに代表されるリアルタイム描画に特化した演算装置あるいはプロセッサ
	
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain;	// IDXGISwapChainインターフェイス
	// IDXGISwapChainインターフェイスは、出力に表示する前に、レンダリングされたデータを
	// 格納するための1つ以上のサーフェスを実装します
	
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;	// レンダーターゲットビューインターフェイス
	// レンダーターゲットビューインターフェイスは、レンダリング中にアクセスできる
	// レンダーターゲットサブリソースを識別します
	// ・色を書き込むキャンバス
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;	// 深度ステンシルビューインターフェイス
	// 深度ステンシルビューインターフェイスは、
	// 深度ステンシルテスト中にテクスチャリソースにアクセスします
	// ・奥行情報を書き込むキャンバス
#endif


	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_states[4]; // ID3D11DepthStencilStateインターフェイス
	// 深度ステンシル状態インターフェイスは、
	// 出力マージャーステージにバインドできる深度ステンシル状態の説明を保持します
	
	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_states[4]; // ID3D11BlendStateインターフェイス
	// blend-stateインターフェイスは、output-mergerステージにバインドできるブレンド状態の説明を保持します

	std::unique_ptr<sprite> sprites[8];	// sprite*型配列

	std::unique_ptr<sprite_batch> sprite_batches[8];

	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_states[3];

	struct scene_constants
	{
		DirectX::XMFLOAT4X4 view_projection;	// ビュー・プロジェクション変換行列
		DirectX::XMFLOAT4 light_direction;		// ライトの向き
		DirectX::XMFLOAT4 camera_position;		// カメラ位置
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffers[8];

	std::unique_ptr<geometric_primitive> geometric_primitives[8];

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


#if 1
	void SetImguiStyle()
	{
		//mStyle.WindowMinSize = ImVec2(160, 20);
		//mStyle.FramePadding = ImVec2(4, 2);
		//mStyle.ItemSpacing = ImVec2(6, 2);
		//mStyle.ItemInnerSpacing = ImVec2(6, 4);
		//mStyle.Alpha = 0.95f;
		//mStyle.WindowRounding = 4.0f;
		//mStyle.FrameRounding = 2.0f;
		//mStyle.IndentSpacing = 6.0f;
		//mStyle.ItemInnerSpacing = ImVec2(2, 4);
		//mStyle.ColumnsMinSpacing = 50.0f;
		//mStyle.GrabMinSize = 14.0f;
		//mStyle.GrabRounding = 16.0f;
		//mStyle.ScrollbarSize = 12.0f;
		//mStyle.ScrollbarRounding = 16.0f;

		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
		//style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);


	}
	void SetImguiStyle1()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.Alpha = 1.0;
		//style.WindowFillAlphaDefault = 0.83;
		//style.ChildWindowRounding = 3;
		style.WindowRounding = 3;
		style.GrabRounding = 1;
		style.GrabMinSize = 20;
		style.FrameRounding = 3;


		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
		//style.Colors[ImGuiCol_ComboBg] = ImVec4(0.16f, 0.24f, 0.22f, 0.60f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.65f, 0.65f, 0.46f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 1.00f, 1.00f, 0.43f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.62f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
		//style.Colors[ImGuiCol_Column] = ImVec4(0.00f, 0.50f, 0.50f, 0.33f);
		//style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.00f, 0.50f, 0.50f, 0.47f);
		//style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.00f, 0.70f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
		//style.Colors[ImGuiCol_CloseButton] = ImVec4(0.00f, 0.78f, 0.78f, 0.35f);
		//style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.00f, 0.78f, 0.78f, 0.47f);
		//style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.00f, 0.78f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
		//style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.00f, 0.13f, 0.13f, 0.90f);
		//style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.04f, 0.10f, 0.09f, 0.51f);
	}
	void SetImguiStyle2()
	{
		//ImGuiIO& io = ImGui::GetIO();

		//io.Fonts->Clear();
		//io.Fonts->AddFontFromFileTTF(ofToDataPath("fonts/OpenSans-Light.ttf", true).c_str(), 16);
		//io.Fonts->AddFontFromFileTTF(ofToDataPath("fonts/OpenSans-Regular.ttf", true).c_str(), 16);
		//io.Fonts->AddFontFromFileTTF(ofToDataPath("fonts/OpenSans-Light.ttf", true).c_str(), 32);
		//io.Fonts->AddFontFromFileTTF(ofToDataPath("fonts/OpenSans-Regular.ttf", true).c_str(), 11);
		//io.Fonts->AddFontFromFileTTF(ofToDataPath("fonts/OpenSans-Bold.ttf", true).c_str(), 11);
		//io.Fonts->Build();

		ImGuiStyle* style = &ImGui::GetStyle();

		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.39f, 0.38f, 0.77f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.92f, 0.91f, 0.88f, 0.70f);
		//style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.58f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.92f, 0.91f, 0.88f, 0.92f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.84f, 0.83f, 0.80f, 0.65f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.99f, 1.00f, 0.40f, 0.78f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.47f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.00f, 0.00f, 0.21f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.90f, 0.91f, 0.00f, 0.78f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_ComboBg] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.25f, 1.00f, 0.00f, 0.80f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.00f, 0.00f, 0.14f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.00f, 0.00f, 0.14f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.99f, 1.00f, 0.22f, 0.86f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.25f, 1.00f, 0.00f, 0.76f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 1.00f, 0.00f, 0.86f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_Column] = ImVec4(0.00f, 0.00f, 0.00f, 0.32f);
		//style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.25f, 1.00f, 0.00f, 0.78f);
		//style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.04f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25f, 1.00f, 0.00f, 0.78f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
		//style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
		//style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		//style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
	}
	void SetImguiStyle3()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		// light style from Pacôme Danhiez (user itamago) https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
		style.Alpha = imgui_debug.imgui_color;
		style.FrameRounding = 3.0f;
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
		//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		//style.Colors[ImGuiCol_ComboBg] = ImVec4(0.86f, 0.86f, 0.86f, 0.99f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		//style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		//style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
		//style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		//style.Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 0.50f);
		//style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
		//style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		//style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

		//	if (bStyleDark_)
		//	{
		//		for (int i = 0; i <= ImGuiCol_COUNT; i++)
		//		{
		//			ImVec4& col = style.Colors[i];
		//			float H, S, V;
		//			ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);
		//	
		//			if (S < 0.1f)
		//			{
		//				V = 1.0f - V;
		//			}
		//			ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
		//			if (col.w < 1.00f)
		//			{
		//				col.w *= alpha_;
		//			}
		//		}
		//	}
		//	else
		//	{
		//		for (int i = 0; i <= ImGuiCol_COUNT; i++)
		//		{
		//			ImVec4& col = style.Colors[i];
		//			if (col.w < 1.00f)
		//			{
		//				col.x *= alpha_;
		//				col.y *= alpha_;
		//				col.z *= alpha_;
		//				col.w *= alpha_;
		//			}
		//		}
		//	}
		//}
	}
	void SetImguiStyle4()
	{
#if 1
		// FIXME: those should become parameters to the function
		static int hue = 140;
		static float col_main_sat = 180.f / 255.f;
		static float col_main_val = 161.f / 255.f;
		static float col_area_sat = 124.f / 255.f;
		static float col_area_val = 100.f / 255.f;
		static float col_back_sat = 59.f / 255.f;
		static float col_back_val = 40.f / 255.f;

		ImGui::Begin("Hue Style");
		ImGui::SliderInt("master hue", &hue, 0, 255);

		float dummy;
		ImVec4 rgb;
		//ImGui::ColorEditMode(ImGuiColorEditMode_HSV);

		ImGui::ColorConvertHSVtoRGB(hue / 255.f, col_main_sat, col_main_val, rgb.x, rgb.y, rgb.z);
		ImGui::ColorEdit3("main", &rgb.x);
		ImGui::ColorConvertRGBtoHSV(rgb.x, rgb.y, rgb.z, dummy, col_main_sat, col_main_val);

		ImGui::ColorConvertHSVtoRGB(hue / 255.f, col_area_sat, col_area_val, rgb.x, rgb.y, rgb.z);
		ImGui::ColorEdit3("area", &rgb.x);
		ImGui::ColorConvertRGBtoHSV(rgb.x, rgb.y, rgb.z, dummy, col_area_sat, col_area_val);

		ImGui::ColorConvertHSVtoRGB(hue / 255.f, col_back_sat, col_back_val, rgb.x, rgb.y, rgb.z);
		ImGui::ColorEdit3("back", &rgb.x);
		ImGui::ColorConvertRGBtoHSV(rgb.x, rgb.y, rgb.z, dummy, col_back_sat, col_back_val);

		ImGui::End();
#endif

		ImGuiStyle& style = ImGui::GetStyle();

		ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
		ImVec4 col_main = ImColor::HSV(hue / 255.f, col_main_sat, col_main_val);
		ImVec4 col_back = ImColor::HSV(hue / 255.f, col_back_sat, col_back_val);
		ImVec4 col_area = ImColor::HSV(hue / 255.f, col_area_sat, col_area_val);

		style.Alpha = imgui_debug.imgui_color;
		style.Colors[ImGuiCol_Text] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(col_text.x, col_text.y, col_text.z, 0.58f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(col_back.x, col_back.y, col_back.z, 1.00f);
		//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(col_area.x, col_area.y, col_area.z, 0.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(col_text.x, col_text.y, col_text.z, 0.30f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.68f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.45f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(col_main.x, col_main.y, col_main.z, 0.35f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 0.57f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.31f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		//style.Colors[ImGuiCol_ComboBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(col_main.x, col_main.y, col_main.z, 0.80f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.24f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(col_main.x, col_main.y, col_main.z, 0.44f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(col_main.x, col_main.y, col_main.z, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		//style.Colors[ImGuiCol_Column] = ImVec4(col_text.x, col_text.y, col_text.z, 0.32f);
		//style.Colors[ImGuiCol_ColumnHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0.78f);
		//style.Colors[ImGuiCol_ColumnActive] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(col_main.x, col_main.y, col_main.z, 0.20f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		//style.Colors[ImGuiCol_CloseButton] = ImVec4(col_text.x, col_text.y, col_text.z, 0.16f);
		//style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0.39f);
		//style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
		//style.Colors[ImGuiCol_TooltipBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.92f);
		//style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}
#endif

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
				PostMessage(hwnd, WM_CLOSE, 0, 0);
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
		float angle;		// 角度
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