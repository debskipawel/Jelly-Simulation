#define INPUT_PATCH_SIZE 16
#define OUTPUT_PATCH_SIZE 16

struct HSInput
{
    float4 sv_pos : SV_POSITION;
    float4 pos : POSITION;
};

struct DSControlPoint
{
    float4 pos : POSITION;
};

struct HSPatchOutput
{
	float EdgeTessFactor[4]			: SV_TessFactor;
	float InsideTessFactor[2]		: SV_InsideTessFactor;
};


HSPatchOutput CalcHSPatchConstants(InputPatch<HSInput, INPUT_PATCH_SIZE> ip, uint PatchID : SV_PrimitiveID)
{
    HSPatchOutput o;

    o.EdgeTessFactor[0] = o.EdgeTessFactor[1] = o.EdgeTessFactor[2] = o.EdgeTessFactor[3] = 16;
    o.InsideTessFactor[0] = o.InsideTessFactor[1] = 16;

	return o;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(OUTPUT_PATCH_SIZE)]
[patchconstantfunc("CalcHSPatchConstants")]
DSControlPoint main(InputPatch<HSInput, INPUT_PATCH_SIZE> ip, uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID)
{
    DSControlPoint o;
	o.pos = ip[i].pos;

	return o;
}
