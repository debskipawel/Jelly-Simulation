struct DS_OUTPUT
{
	float4 vPosition  : SV_POSITION;
};

struct HS_CONTROL_POINT_OUTPUT
{
	float3 pos : WORLD_POS; 
};

struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[4]			: SV_TessFactor;
	float InsideTessFactor[2]		: SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 16

float3 PointOnCurve(float3 cp[4], float t)
{
    float3 a = lerp(cp[0], cp[1], t);
    float3 b = lerp(cp[1], cp[2], t);
    float3 c = lerp(cp[2], cp[3], t);
    float3 d = lerp(a, b, t);
    float3 e = lerp(b, c, t);
	
    return lerp(d, e, t);
}

float3 PointOnSurface(const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch, float2 uv)
{
    float3 uRow1[] = { patch[0].pos,  patch[1].pos,  patch[2].pos,  patch[3].pos };
    float3 uRow2[] = { patch[4].pos,  patch[5].pos,  patch[6].pos,  patch[7].pos };
    float3 uRow3[] = { patch[8].pos,  patch[9].pos,  patch[10].pos, patch[11].pos };
    float3 uRow4[] = { patch[12].pos, patch[13].pos, patch[14].pos, patch[15].pos };

    float3 newCP[] = { PointOnCurve(uRow1, uv.x), PointOnCurve(uRow2, uv.x), PointOnCurve(uRow3, uv.x), PointOnCurve(uRow4, uv.x) };

    return PointOnCurve(newCP, uv.y);
}

[domain("quad")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float2 uv : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;
    Output.vPosition = float4(PointOnSurface(patch, uv), 1.0);

	return Output;
}
