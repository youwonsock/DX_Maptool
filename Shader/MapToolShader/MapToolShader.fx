#include "Global.fx"

Texture2D Texture0;

PNTOutput VS(PNTTVertex input)
{
    PNTOutput output;
	output.position = input.position;
	output.position = mul(output.position, World);
	output.position = mul(output.position, ViewProjection);
	
    output.uv = input.uv;
	output.normal = input.normal;
	
	return output;
}

float4 PS(PNTOutput input) : SV_TARGET
{
    return Texture0.Sample(LinearSampler, input.uv);
}

float4 PS_RED(PNTOutput input) : SV_TARGET
{
    return float4(1, 0, 0, 1);
}

technique11 T0
{
	PASS_VP(P0, VS, PS)
	PASS_RS_VP(P1, FillModeWireFrame, VS, PS_RED)
};