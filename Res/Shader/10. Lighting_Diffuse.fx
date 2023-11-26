#include "00. Global.fx"

float3 LightDir;     // 태양
float4 LightDiffuse; // 빛의 색
float4 MatrealDiffuse; // 물체가 받아들이는 빛의 양
Texture2D DiffuseMap;

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

// Diffuse(분산광)
// 물체의 표면에서 분산되어 눈으로 바로 들어오는 빛
// 각도에 따라 밝기가 다르다(Lambert 공식)

float4 PS(PNTOutput input) : SV_TARGET
{
    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
    float val = dot(-LightDir, normalize(input.normal));
    
    return color * val * LightDiffuse * MatrealDiffuse;
}

technique11 T0
{
    PASS_VP(p0, VS, PS)
};