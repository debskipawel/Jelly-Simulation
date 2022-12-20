cbuffer world : register(b0)
{
    matrix worldMtx;
    matrix viewMatrix;
    matrix invViewMatrix;
    matrix projMatrix;
};

struct VSInput
{
    float3 pos : POSITION;
};

struct HSInput
{
    float4 sv_pos : SV_POSITION;
    float4 pos : POSITION;
};

HSInput main(VSInput i)
{
    HSInput o;
    o.pos = mul(viewMatrix, mul(worldMtx, float4(i.pos, 1.0f)));
    o.sv_pos = mul(projMatrix, o.pos);
    
    return o;
}
