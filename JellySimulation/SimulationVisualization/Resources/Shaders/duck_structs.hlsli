
struct VSInput
{
    float3 pos : POSITION;
    float3 norm : NORMAL0;
    float2 tex : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float4 worldPos : POSITION0;
    float4 norm : NORMAL0;
    float4 viewVec : VIEW;
    float2 tex : TEXCOORD1;
};