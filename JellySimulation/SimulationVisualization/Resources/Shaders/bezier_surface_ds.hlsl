#define OUTPUT_PATCH_SIZE 16

cbuffer mvpBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix invView;
    matrix proj;
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

struct DSControlPoint
{
    float4 pos : POSITION;
};

struct HSPatchOutput
{
    float edges[4] : SV_TessFactor;
    float insideFactors[2] : SV_InsideTessFactor;
};

float4 DeCasteljau3(int n, float t)
{
    matrix bernsteinBasis =
    {
        { 1.0, 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0, 0.0 },
    };

    float u = 1.0 - t;

    for (int j = 1; j <= n; j++)
    {
        bernsteinBasis[j][0] = bernsteinBasis[j - 1][0] * u;

        for (int i = 1; i <= j; i++)
        {
            bernsteinBasis[j][i] = bernsteinBasis[j - 1][i] * u + bernsteinBasis[j - 1][i - 1] * t;
        }
    }

    return float4(bernsteinBasis[n][0], bernsteinBasis[n][1], bernsteinBasis[n][2], bernsteinBasis[n][3]);
}

float4 PointOnBezier(const OutputPatch<DSControlPoint, OUTPUT_PATCH_SIZE> input, float2 uv)
{
    float4 bernsteinU = DeCasteljau3(3, uv.x);
    float4 bernsteinV = DeCasteljau3(3, uv.y);
    
    float4 result = 0.0;
    
    for (int i = 0; i < 4; i++)
    {
        float4 partialResult = 0.0;
        
        for (int j = 0; j < 4; j++)
        {
            partialResult += input[4 * i + j].pos * bernsteinU[j];
        }

        result += partialResult * bernsteinV[i];
    }

    return result;
}

float3 NormalOnBezier(const OutputPatch<DSControlPoint, OUTPUT_PATCH_SIZE> input, float2 uv, inout TangentSystem tang)
{
    tang = (TangentSystem) 0;
    
    float4 bernsteinU = DeCasteljau3(3, uv.x);
    float4 bernsteinV = DeCasteljau3(3, uv.y);
    
    float4 bernsteinDiffU = DeCasteljau3(2, uv.x);
    float4 bernsteinDiffV = DeCasteljau3(2, uv.y);
    
    float4 uCurveCP[4];
    float4 vCurveCP[4];
    
    for (int i = 0; i < 4; i++)
    {
        uCurveCP[i] = 0.0f;
        vCurveCP[i] = 0.0f;
        
        for (int j = 0; j < 4; j++)
        {
            uCurveCP[i] += input[4 * j + i].pos * bernsteinV[j];
            vCurveCP[i] += input[4 * i + j].pos * bernsteinU[j];
        }
    }

    float4 uDerivative = 0.0f, vDerivative = 0.0f;
    
    for (int k = 0; k < 3; k++)
    {
        uDerivative += 3.0f * (uCurveCP[k + 1] - uCurveCP[k]) * bernsteinDiffU[k];
        vDerivative += 3.0f * (vCurveCP[k + 1] - vCurveCP[k]) * bernsteinDiffV[k];
    }
    
    tang.T = uDerivative;
    tang.B = vDerivative;
    tang.N = cross(normalize(uDerivative).xyz, normalize(vDerivative).xyz);

    return tang.N;
}

[domain("quad")]
PSInput main(
	HSPatchOutput input,
	float2 uv : SV_DomainLocation,
	const OutputPatch<DSControlPoint, OUTPUT_PATCH_SIZE> patch)
{
    PSInput o;
    TangentSystem sys;
    
    float4 viewPos = PointOnBezier(patch, uv);
    float3 normal = NormalOnBezier(patch, uv, sys);
    
    sys.N = normalize(mul(invView, float4(sys.N, 0.0)).xyz);
    sys.T = normalize(mul(invView, float4(sys.T, 0.0)).xyz);
    sys.B = normalize(mul(invView, float4(sys.B, 0.0)).xyz);
    
    o.pos = mul(proj, viewPos);
    o.viewPos = viewPos;
    o.tangent = sys;
    o.worldPos = mul(invView, viewPos);
    
    o.camPos = mul(invView, float4(0.0, 0.0, 0.0, 1.0));
    
	return o;
}
