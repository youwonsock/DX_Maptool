#include "Global.fx"

Texture2D MapBaseTexture;
Texture2D MapAlphaTexture;

Texture2D Texture1; // color.r
Texture2D Texture2; // color.g
Texture2D Texture3; // color.b
Texture2D Texture4; // color.a

// use color to textures alpha value

PNCTOutput VS(PNCTVertex input)
{
    PNCTOutput output;
	output.position = input.position;
	output.position = mul(output.position, World);
	output.position = mul(output.position, ViewProjection);
	
    // need to make uv pos
    output.uv = input.uv;
    
	output.normal = input.normal;
    output.color = input.color; // current not use;
	
	return output;
}

float4 PS(PNCTOutput input) : SV_TARGET
{
    float4 result;
    float4 baseColor = MapBaseTexture.Sample(LinearSampler, input.uv);
	float4 alpha = MapAlphaTexture.Sample(LinearSampler, input.uv);
    float4 texColor1 = Texture1.Sample(LinearSampler, input.uv);
    float4 texColor2 = Texture2.Sample(LinearSampler, input.uv);
    float4 texColor3 = Texture3.Sample(LinearSampler, input.uv);
    float4 texColor4 = Texture4.Sample(LinearSampler, input.uv);
	
	result = lerp(baseColor, texColor1, alpha.r);
    result = lerp(result, texColor2, alpha.g);
    result = lerp(result, texColor3, alpha.b);
    result = lerp(result, texColor4, alpha.a);
	
    return result;
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