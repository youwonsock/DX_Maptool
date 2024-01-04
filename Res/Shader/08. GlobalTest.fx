#include "00. Global.fx"

PNTOutput VS(PNTVertex input)
{
    PNTOutput output;
	output.position = input.position;
	output.position = mul(output.position, World);
	output.position = mul(output.position, ViewProjection);
	
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3)World);
    
	return output;
}

Texture2D Texture0;

float4 PS(PNTOutput input) : SV_TARGET
{
    return Texture0.Sample(LinearSampler, input.uv);
}

technique11 T0
{
    PASS_VP(p0, VS, PS)
};