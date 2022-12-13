struct Light
{
    float4 pos;
    float4 color;
    float4 coeff;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float4 worldPos : WORLD_POS;
    float4 normal : NORMAL;
    float4 view : VIEW;
};

float4 main(VSOutput i) : SV_TARGET
{
    float4 surfaceColor = float4(0.5, 0.5, 1.0, 1.0);
    Light sceneLight =
    {
        float4(0.0, -4.0, 1.0, 1.0),
        float4(1.0, 1.0, 1.0, 1.0),
        float4(0.1, 0.5, 0.2, 2.0)
    };
    
    float4 diffuse = float4(0.0, 0.0, 0.0, 0.0);
    float4 specular = float4(0.0, 0.0, 0.0, 0.0);
    
    float4 N = normalize(i.normal);
    float4 L = normalize(sceneLight.pos - i.worldPos);
		
    float LN = clamp(dot(L, N), 0.0, 1.0);
    float RV = clamp(dot(-reflect(L, N), i.view), 0.0, 1.0);
		
    diffuse += LN * sceneLight.color;
    specular += pow(RV, sceneLight.coeff.w) * sceneLight.color;
    
    float4 result = sceneLight.coeff.x * sceneLight.color + sceneLight.coeff.y * diffuse * surfaceColor + sceneLight.coeff.z * specular;
    
    return float4(result.xyz, 1.0);
}