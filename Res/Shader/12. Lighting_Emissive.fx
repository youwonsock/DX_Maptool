#include "00. Global.fx"

float4 MatrealEmissive;

Texture2D DiffuseMap;

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

// Emissive(림 라이트)
// 외곽선을 구할 때 사용
float4 PS(MeshOutput input) : SV_TARGET
{
    float3 cameraPosition = -View._41_42_43;
    float3 eye = normalize(cameraPosition - input.worldPosition);
    
    float val = saturate(dot(input.normal, eye));
    float emissive = 1.0f - val;
    
    // smoothstep : 값 
    emissive = smoothstep(0.0f, 1.0f, emissive);
    emissive = pow(emissive, 2.0f);
    
    float4 color = MatrealEmissive * emissive;
    return color;
}

technique11 T0
{
    PASS_VP(p0, VS, PS)
};