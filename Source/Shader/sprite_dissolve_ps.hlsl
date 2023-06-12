#include "sprite_dissolve.hlsli"

Texture2D color_map : register(t0);
SamplerState color_sampler_state : register(s0);
Texture2D mask_texture : register(t1);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = color_map.Sample(color_sampler_state,pin.texcoord) * pin.color;

    // �}�X�N�摜����ԐF���擾
    float mask = mask_texture.Sample(color_sampler_state, pin.texcoord).r;

    //float mask_value = mask_texture.Sample(color_sampler_state, pin.texcoord);
    
    // step�֐���p���ē��ߒl��0/1�ɂ���
    //float alpha = step(mask, parameters.x);
    float alpha = step(parameters.x, mask);
    //float alpha = step(parameters.x, mask_value);
    
    // ���̏���
    float edgeValue = step(mask - parameters.x, parameters.x) * step(parameters.x, mask) * step(mask, parameters.x + parameters.z);


    color.rgb += edgeColor.rgb * edgeValue;
    alpha = saturate(alpha + edgeValue);

    // color�̓��ߒl�ɏ�Z����
    color.a *= alpha;

    

    // �F�X�������Ă��炢�������Ƀt�F�[�h�A�E�g�I�Ȃ��ꂽ
    //float temp = step(parameters.y, mask_value);
    //float temp1 = step(alpha, temp);
    //color.r *= temp1;
    //color.g *= temp1;
    //color.b *= temp1;

    return color;
}