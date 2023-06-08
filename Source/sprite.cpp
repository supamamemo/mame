#include "sprite.h"
#include "misc.h"
#include <sstream>
 
#include "../External/DirectXTK-main/Inc/WICTextureLoader.h"

#include "texture.h"
#include "shader.h"

void sprite::render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh, float r, float g, float b, float a, float angle)
{
    render(immediate_context, dx, dy, dw, dh, r, g, b, a, angle, 0.0f, 0.0f, static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height));
}

void sprite::render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh, float r, float g, float b, float a, float angle, float sx, float sy, float sw, float sh)
{
    // <1> スクリーン（ビューポート）のサイズを取得する
#if 1
    // D3D11_VIEWPORT ビューポートの寸法を定義します
    D3D11_VIEWPORT viewport{};
    UINT num_viewports{ 1 };
    // <RSGetViewport> ラスタライザーステージにバインドされたビューポートの配列を取得します
    immediate_context->RSGetViewports(&num_viewports, &viewport);
#endif
    // <2> renderメンバ関数の引数(dx,dy,dw,dh)から短形の角頂点の位置（スクリーン座標系）を計算する
#if 1
    // (x0, y0) *----* (x1, y1)
    //          |   /|
    //          |  / |
    //          | /  |
    //          |/   |
    // (x2, y2) *----* (x3, y3)

    // left-top
    float x0{ dx };
    float y0{ dy };
    // right-top
    float x1{ dx + dw };
    float y1{ dy };
    // left-bottom
    float x2{ dx };
    float y2{ dy + dh };
    // right-bottom
    float x3{ dx + dw };
    float y3{ dy + dh };
#endif

    // 矩形回転
#if 1
    auto rotate = [](float& x, float& y, float cx, float cy, float angle)
    {
        x -= cx;
        y -= cy;

        float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
        float sin{ sinf(DirectX::XMConvertToRadians(angle)) };
        float tx{ x }, ty{ y };
        x = cos * tx + -sin * ty;
        y = sin * tx + cos * ty;

        x += cx;
        y += cy;
    };
    // 回転の中心を短形の中心点にした場合
    float cx = dx + dw * 0.5f;
    float cy = dy + dh * 0.5f;
    rotate(x0, y0, cx, cy, angle);
    rotate(x1, y1, cx, cy, angle);
    rotate(x2, y2, cx, cy, angle);
    rotate(x3, y3, cx, cy, angle);
#endif

    // <3> スクリーン座標系からNDCへの座標変換をおこなう
#if 1
    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;
#endif
    // <4> 計算結果で頂点バッファオブジェクトを更新する
#if 1
    HRESULT hr{ S_OK };
    // D3D11_MAPPED_SUBRESOURCE構造体 サブリソースデータへのアクセスを提供します
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    if (vertices != nullptr)
    {
        vertices[0].position = { x0, y0, 0 };
        vertices[1].position = { x1, y1, 0 };
        vertices[2].position = { x2, y2, 0 };
        vertices[3].position = { x3, y3, 0 };
        vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r,g,b,a };

        vertices[0].texcord = { sx / texture2d_desc.Width, sy / texture2d_desc.Height };
        vertices[1].texcord = { (sx + sw) / texture2d_desc.Width, sy / texture2d_desc.Height };
        vertices[2].texcord = { sx / texture2d_desc.Width, (sy + sh) / texture2d_desc.Height };
        vertices[3].texcord = { (sx + sw) / texture2d_desc.Width, (sy + sh) / texture2d_desc.Height };
    }

    immediate_context->Unmap(vertex_buffer.Get(), 0);
#endif    

    // <1> 頂点バッファーのバインド
    UINT stride{ sizeof(vertex) };
    UINT offset{ 0 };
    // <IASetVertexBuffers()> 頂点バッファーのCPU記述子ハンドルを設定します
    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

    // <2> プリミティブタイプ及びデータの序列に関する情報のバインド
    // <IASetPrimitiveTopology> プリミティブ型に関する情報と、
    // 入力アセンブラーステージの入力データを記述を記述するデータ順序をバインドします
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // <3> 入力レイアウトオブジェクトのバインド
    // <IASetInputLayout> 入力レイアウトオブジェクトを入力アセンブラーステージをバインドします
    immediate_context->IASetInputLayout(input_layout.Get());

    // <4> シェーダーのバインド
    immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);


    // シェーダーリソースのバインド
    immediate_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());

    // <5> プリミティブの描画
    immediate_context->Draw(4, 0);
}

void sprite::render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh)
{
    render(immediate_context, dx, dy, dw, dh, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height));
}


// コンストラクタ
sprite::sprite(ID3D11Device* device, const wchar_t* filename)
{
    HRESULT hr{ S_OK };

    // <1> 頂点情報のセット
    vertex vertices[]
    {
        { { -1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 0, 0 } },
        { { +1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 1, 0 } },
        { { -1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 0, 1 } },
        { { +1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 1, 1 } },
    };


    // <2> 頂点バッファオブジェクトの生成
#if 1
    // D3D11_BUFFER_DESC構造体 
    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(vertices);           // バッファーのサイズ(バイト単位)
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC;            // バッファーの読み取りと書き込み方法を特定します
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;   // バッファーをパイプラインにバインドする方法を特定する
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;// CPUアクセスフラグまた穂CPUアクセスが必要ない場合は０
    buffer_desc.MiscFlags = 0;                          // その他のフラグまたは未使用の場合は０
    buffer_desc.StructureByteStride = 0;                // バッファーが構造化バッファーを表す場合の
                                                        // バッファー構造内の各要素のサイズ
                                                        
    // D3D11_SUBRESOURCE_DATA構造体 サブリソースを初期化するためのデータを指定します
    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = vertices;    // 初期化データへのポインター 
    subresource_data.SysMemPitch = 0;       // テクスチャの１行の先頭から次の行までの距離(バイト単位)
    subresource_data.SysMemSlicePitch = 0;  // 1つの深度レベルの先頭から次の深さレベルまでの距離(バイト単位)

    // <CreateBuffer> 
    // バッファー（頂点バッファー、インデックスバッファー、またはシェーダー定数バッファー）を作成します
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
#endif
    


    // <4> 入力レイアウトオブジェクトの生成
#if 0
    // D3D11_INPUT_ELEMENT_DESC構造体
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
#endif

#if 0
    // シェーダー関連
    create_vs_from_cso(device, "sprite_vs.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
    create_ps_from_cso(device, "sprite_ps.cso", pixel_shader.GetAddressOf());

    create_vs_from_cso(device, "UVScroll_vs.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));

    create_ps_from_cso(device, "UVScroll_ps.cso", pixel_shader.GetAddressOf());
#endif

    // テクスチャのロード
    load_texture_from_file(device, filename, shader_resource_view.GetAddressOf(), &texture2d_desc);
}

// デストラクタ
sprite::~sprite()
{
}
