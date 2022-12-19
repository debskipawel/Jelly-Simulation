#include "duck_structs.hlsli"

sampler samp : register(s0);
texture2D duckTexure : register(t0);

cbuffer cbLights : register(b0)
{
    float4 lightPositions[1];
};


static const float3 ambientColor = float3(0.5f, 0.5f, 0.5f);
static const float3 lightColor = float3(1.0f, 0.0f, 1.0f);
static const float kd = 0.5, ks = 0.5f, m = 100.0f;

float4 main(PSInput input) : SV_TARGET
{
    //return float4(input.norm.xyz, 1.0f);
    float3 viewVec = normalize(input.viewVec);
    float3 normal = normalize(input.norm);
    float3 surfaceColor = float3(1.0, 0, 0);
    surfaceColor = duckTexure.Sample(samp, input.tex).rgb;
    
    float3 color = surfaceColor * ambientColor;
    for (int i = 0; i < 1; ++i)
    {
        float3 lightPosition = lightPositions[i];
        float3 lightVec = normalize(lightPosition - input.worldPos);
        float3 halfVec = normalize(viewVec + lightVec);
        color += lightColor * surfaceColor.xyz * kd * saturate(dot(normal, lightVec)); //diffuse color
        float nh = dot(normal, halfVec);
        nh = saturate(nh);
        nh = pow(nh, m);
        nh *= ks;
        color += lightColor * nh;
    }
       
    
    return float4(saturate(color), 1.0);
}