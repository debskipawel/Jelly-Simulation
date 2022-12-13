cbuffer mvp : register(b0)
{
    matrix view;
    matrix proj;
}

struct PSInput
{
    float4 p : SV_Position;
    float4 near : NEAR;
    float4 far : FAR;
};

struct PSOutput
{
    float4 color : SV_Target;
    float depth : SV_Depth;
};

float4 grid(float4 gridPos, float scale)
{
    float2 coord = gridPos.xy * scale;
    float2 derivative = fwidth(coord);
    
    float minX = min(derivative.x, 1);
    float minY = min(derivative.y, 1);
    
    float2 grid = abs(frac(coord - 0.5) - 0.5) / derivative;
    
    float lin = min(grid.x, grid.y);
    
    float4 color = float4(0.2, 0.2, 0.2, 1.0 - min(lin, 1.0));
    
    const float axisThickness = 0.5;
    
    if (gridPos.x > -axisThickness * minX && gridPos.x < axisThickness * minX)
    {
        color.y = 1.0;
    }
    
    if (gridPos.y > -axisThickness * minY && gridPos.y < axisThickness * minY)
    {
        color.x = 1.0;
    }
    
    return color;
}

float computeLinearDepth(float4 pos)
{
    const float near = 0.01, far = 100.0;
    
    float4 clip_space_pos = mul(proj, mul(view, pos));
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0;
    float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near));
    
    return linearDepth / far;
}

PSOutput main(PSInput i)
{
    PSOutput o = (PSOutput) 0;
    
    float t = -i.near.z / (i.far.z - i.near.z);
    
    float4 gridPos = i.near + t * (i.far - i.near);
    
    o.color = grid(gridPos, 1.0f) * float(t > 0);
    
    float linearDepth = computeLinearDepth(gridPos);
    float fading = max(0, (0.5 - linearDepth));
    
    o.color.a *= fading;
    
    float4 projPos = mul(proj, mul(view, gridPos));
    o.depth = projPos.z / projPos.w;
    
    return o;
}