#include "00. Global.fx"
#include "00. Light.fx"

MeshOutput VS(PNTTVertex input)
{
    MeshOutput output;
	output.position = input.position;
	output.position = mul(output.position, World);
    output.worldPosition = output.position.xyz;
	output.position = mul(output.position, ViewProjection);
	
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3)World);
    output.tangent = mul(input.tangent, (float3x3)World);
    
	return output;
}

float4 PS(MeshOutput input) : SV_TARGET
{
    ComputeNormalMapping(input.normal, input.tangent, input.uv);
    
    return ComputeLight(input.normal, input.uv, input.worldPosition);
}

technique11 T0
{
    PASS_VP(p0, VS, PS)
};