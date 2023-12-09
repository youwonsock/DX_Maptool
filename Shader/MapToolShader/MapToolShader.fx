#include "Global.fx"

Texture2D Texture0;

PNCTOutput VS(PNCTVertex input)
{
    PNCTOutput output;
	output.position = input.position;
	output.position = mul(output.position, World);
	output.position = mul(output.position, ViewProjection);
	
    output.uv = input.uv;
	output.normal = input.normal;
	output.color = input.color;
	
	return output;
}

float4 PS(PNCTOutput input) : SV_TARGET
{
//    return Texture0.Sample(LinearSampler, input.uv);
	
    return float4(1,1,0,1);
}

float4 PS_RED(PNCTOutput input) : SV_TARGET
{
    return float4(1, 0, 0, 1);
}

technique11 T0
{
	PASS_VP(P0, VS, PS)
	PASS_RS_VP(P1, FillModeWireFrame, VS, PS_RED)
};