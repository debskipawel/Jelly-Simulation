cbuffer mvpBuffer : register(b0)
{
    matrix view;
    matrix proj;
}

struct VSInput
{
    float3 pos : POSITION;
    float startTime : START_TIME;
    float endTime : END_TIME;
    float percentage : PERCENTAGE;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 projPos : PROJ_POS;
    float4 worldPos : WORLD_POS;
    float startTime : START_TIME;
    float endTime : END_TIME;
    float percentage : PERCENTAGE;
};

VSOutput main(VSInput i)
{
    VSOutput o = (VSOutput) 0;
    
    o.worldPos = float4(i.pos, 1.0);
    o.pos = o.projPos = mul(proj, mul(view, o.worldPos));
    
    o.startTime = i.startTime;
    o.endTime = i.endTime;
    o.percentage = i.percentage;
    
    return o;
}