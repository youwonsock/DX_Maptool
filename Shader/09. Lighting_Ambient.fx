#include "00. Global.fx"

float4 LightAmbient;
float4 MaterialAmbient;

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
// Ambient (주변광/ 환경광)
// 수많은 반사를 거쳐서 광원이 불분명한 빛
// 일정한 밝기와 색으로 표현

float4 PS(PNTOutput input) : SV_TARGET
{
    float4 color = LightAmbient * MaterialAmbient;
    return color * Texture0.Sample(LinearSampler, input.uv);
}

technique11 T0
{
    PASS_VP(p0, VS, PS)
};