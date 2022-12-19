cbuffer color : register(b0)
{
    float4 color;
}

struct TangentSystem
{
    float3 N : NORMAL;
    float3 T : TANGENT;
    float3 B : BINORMAL;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float4 viewPos : VIEWPOS;
    float4 worldPos : WORLDPOS;
    TangentSystem tangent;
    float3 camPos : CAMERA;
};

float4 main(PSInput i) : SV_TARGET
{
    float3 LIGHT_POS = float3(1.0, -3.0, 2.0);
    float4 LIGHT_COLOR = float4(1.0, 1.0, 1.0, 1.0);

    float ka = 0.1, kd = 0.5, ks = 0.2, m = 100.0;

    
    float3 V = normalize(i.camPos - i.worldPos.xyz);
    float3 L = normalize(LIGHT_POS - i.worldPos.xyz);
    float3 H = normalize(L + V);
    float3 N = normalize(i.tangent.N);
	
    float4 albedo = color;
    
    // ambient
    float4 ambient = ka * albedo;
    
    // diffuse
    float nl = clamp(dot(L, N), 0.0, 1.0);
    float4 diffuse = kd * nl * LIGHT_COLOR * albedo;
    
    // specular
    float nh = clamp(dot(N, H), 0.0, 1.0);
    float4 specular = ks * pow(nh, 4 * m) * LIGHT_COLOR;
    
    float4 final = ambient + diffuse + specular;
    final.w = 1;
    
    return final;
}
