#pragma once

#include <memory>
#include <d3d11.h>
#include <wrl.h>
#include "../../shader.h"

class Graphics
{
public:
    Graphics(HWND hWnd);
    ~Graphics();

    // �C���X�^���X�擾
    static Graphics& Instance() { return *instance; }

    // �f�o�C�X�擾
    ID3D11Device* GetDevice() const { return device.Get(); }

    // �f�o�C�X�R���e�L�X�g�擾
    ID3D11DeviceContext* GetDeviceContext() const { return immediateContext.Get(); }

    // �X���b�v�`�F�[���擾
    IDXGISwapChain* GetSwapChain() const { return swapchain.Get(); }

    // �����_�[�^�[�Q�b�g�r���[�擾
    ID3D11RenderTargetView* GetRenderTargetView() const { return renderTargetView.Get(); }

    // �f�v�X�X�e���V���r���[�擾
    ID3D11DepthStencilView* GetDepthStencilView() const { return depthStencilView.Get(); }

    // �V�F�[�_�[�擾
    Shader* GetShader()const { return shader.get(); }

    // �X�N���[�����擾
    float GetScreenWidth() const { return screenWidth; }

    // �X�N���[�������擾
    float GetScreenHeight() const { return screenHeight; }

private:
    static Graphics* instance;

    Microsoft::WRL::ComPtr<ID3D11Device>            device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     immediateContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain>          swapchain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D>         depthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  depthStencilView;

    std::unique_ptr<Shader> shader;

    float screenWidth;
    float screenHeight;
};

