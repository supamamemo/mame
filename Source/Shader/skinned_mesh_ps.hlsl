#include "skinned_mesh.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState sampler_states[3]:register(s0);
Texture2D texture_maps[4]:register(t0);
Texture2D mask_texture : register(t15);
float4 main(VS_OUT pin) :SV_TARGET
{
    float mask_value = mask_texture.Sample(sampler_states[0],pin.texcoord) * pin.color;

    float4 color = texture_maps[0].Sample(sampler_states[ANISOTROPIC],pin.texcoord);
    //float alpha = color.a;
#if 1
    // Inverse gamma process
    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, GAMMA);
#endif

    float3 N = normalize(pin.world_normal.xyz);
    float3 T = normalize(pin.world_tangent.xyz);
    float sigma = pin.world_tangent.w;
    T = normalize(T - N * dot(N, T));
    float3 B = normalize(cross(N, T) * sigma);

    float4 normal = texture_maps[1].Sample(sampler_states[LINEAR], pin.texcoord);
    normal = (normal * 2.0) - 1.0;
    N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));

    float3 L = normalize(-light_direction.xyz);
    float3 diffuse = color.rgb * max(0, dot(N, L));
    float3 V = normalize(camera_position.xyz - pin.world_position.xyz);
    float3 specular = pow(max(0, dot(N, normalize(V + L))), 128);

    // dissolve
    float alpha = step(parameters.x, mask_value);


    //color.a *= alpha;
    //return color;
    

    return color * pin.color;
    //return float4(diffuse, alpha) * pin.color;
    
    return float4(diffuse + specular, alpha) * pin.color;
}