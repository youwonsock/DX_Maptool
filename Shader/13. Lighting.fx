#include "00. Global.fx"
#include "00. Light.fx"

MeshOutput VS(PNTVertex input)
{
    MeshOutput output;
	output.position = input.position;
	output.position = mul(output.position, World);
	output.position = mul(output.position, ViewProjection);
	
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3)World);
    output.worldPosition = input.position;
    
	return output;
}

float4 PS(MeshOutput input) : SV_TARGET
{
    return ComputeLight(input.normal, input.uv, input.worldPosition);
}

technique11 T0
{
    PASS_VP(p0, VS, PS)
};