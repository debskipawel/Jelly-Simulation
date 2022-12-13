cbuffer mvpBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix invView;
    matrix proj;
}

struct VSInput
{
    float3 pos : POSITION;
    float3 norm : NORMAL;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float4 worldPos : WORLD_POS;
    float4 normal : NORMAL;
    float4 view : VIEW;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    o.worldPos = mul(model, float4(i.pos, 1.0));
    o.pos = mul(proj, mul(view, o.worldPos));
    o.normal = normalize(mul(model, float4(i.norm, 0.0)));   
    o.view = normalize(mul(invView, float4(0.0, 0.0, 0.0, 1.0)) - o.worldPos);
    
    return o;
}