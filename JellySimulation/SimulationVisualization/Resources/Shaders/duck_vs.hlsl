
#include "duck_structs.hlsli"
float4 DeCasteljeu(float4 coef[4], float t)
{
    for (int i = 0; i < 4 - 1; i++)
    {
        int currentSize = 4 - i;
        for (int j = 0; j < currentSize - 1; j++)
        {
            coef[j] = coef[j] * (1.0f - t) + coef[j + 1] * t;
        }
    }
    float4 value = coef[0];
    
    return value;
}


cbuffer mvpBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix invViewMatrix;
    matrix projMatrix;
};

cbuffer controlPointsBuffer : register(b1)
{
    float4 controlPoints[4][4][4];
};

float4 GetPoint(float u, float v, float w)
{
    float4 pW[4];
    for (int i = 0; i < 4; ++i)
    {
        float4 pV[4];
        for (int j = 0; j < 4; ++j)
        {
            float4 pU[4];
            for (int k = 0; k < 4; ++k)
            {
                pU[k] = controlPoints[i][j][k];
            }
            pV[j] = DeCasteljeu(pU, u);
        }
        pW[i] = DeCasteljeu(pV, v);
    }
    float4 distortedPoint = DeCasteljeu(pW, w);
    return distortedPoint;

}

PSInput main(VSInput input)
{
    PSInput o;
    
    float4 undistortedWorldPos = mul(worldMatrix, float4(input.pos, 1.0f));
    
    float u = (undistortedWorldPos.x + 0.5);
    float v = (undistortedWorldPos.y + 0.5);
    float w = (undistortedWorldPos.z + 0.5);
    
  
    float4 distortedPoint = GetPoint(u, v, w);
    
    float4 scaledWorldPos = undistortedWorldPos * 0.9;
    
    float uScaled = (scaledWorldPos.x + 0.5);
    float vScaled = (scaledWorldPos.y + 0.5);
    float wScaled = (scaledWorldPos.z + 0.5);
    
    float4 distortedScaledPoint = GetPoint(uScaled, vScaled, wScaled);
    
    
    o.worldPos = distortedPoint;
    o.pos = mul(projMatrix, mul(viewMatrix, distortedPoint));
    
    
    o.norm = distortedPoint - distortedScaledPoint;
    o.norm = normalize(float4(o.norm.xyz, 0));
    //o.norm = float4(input.norm, 0);
    
    
    float4 camPos = mul(invViewMatrix, float4(0.0f, 0.0f, 0.0f, 1.0f));
    o.viewVec = camPos - o.worldPos;
    o.tex = input.tex;
    
    return o;
}