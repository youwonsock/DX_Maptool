#include "00. Global.fx"

float3 LightDir;     // 태양
float4 LightSpecular; // 빛의 색
float4 MatrealSpecular; // 물체가 받아들이는 빛의 양

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

// Specular (반사광)
// 한방향으로 완전히 반사되는 빛
float4 PS(MeshOutput input) : SV_TARGET
{
    // float3 R = reflect(-LightDir, normalize(input.normal)); // 내장 함수를 이용한 반사 벡터
    float3 R = LightDir - (2 * input.normal * dot(LightDir, input.normal));
    R = normalize(R);
    float3 cameraPos = -View._41_42_43;
    float3 eye = normalize(cameraPos - input.worldPosition);
    
    float val = saturate(dot(R, eye)); // clamp(0, 1);
    float specular = pow(val, 10);     // 반사광의 영역를 강조하기 위해 사용(값이 크면 반사되는 빛의 영역이 작아짐)
    
    float4 color = LightSpecular * MatrealSpecular * specular;
    
    return color;
}

technique11 T0
{
    PASS_VP(p0, VS, PS)
};