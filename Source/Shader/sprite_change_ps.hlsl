#include "sprite.hlsli"

Texture2D color_map:register(t0);
SamplerState point_sampler_state:register(s0);
SamplerState linear_sampler_state:register(s1);
SamplerState anisotropic_sampler_state:register(s2);
float4 main(VS_OUT pin) :SV_TARGET
{
    return color_map.Sample(point_sampler_state, pin.texcoord) * pin.color;
}

//float main(VS_OUT pin) :SV_TARGET
//{
//    return pin.color;
//}

// ���̊�
#if 0
float4 main(VS_OUT pin) :SV_TARGET
{
    return float4(1, 1, 1, 1);
    const float2 center = float2(1280 / 2,720 / 2);
    float distance = length(center - pin.position.xy);
    if (distance > 200)return 1;
    else return float4(1, 0, 0, 1);
}
#endif