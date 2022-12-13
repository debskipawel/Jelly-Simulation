cbuffer invMat : register(b0)
{
    matrix invView;
    matrix invProj;
}

struct VSInput
{
    float3 pos : POSITION;
};

struct VSOutput
{
    float4 p : SV_Position;
    float4 near : NEAR;
    float4 far : FAR;
};

float4 UnprojectPoint(float2 xy, float z)
{
    float4 p = float4(xy.x, xy.y, z, 1.0);
    p = mul(invView, mul(invProj, p));
    p /= p.w;
    
    return p;
}

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    o.near = UnprojectPoint(i.pos.xy, 0.0);
    o.far = UnprojectPoint(i.pos.xy, 1.0);

    o.p = float4(i.pos, 1.0);
    
    return o;
}