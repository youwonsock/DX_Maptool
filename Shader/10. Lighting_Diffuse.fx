#include "00. Global.fx"

float3 LightDir;     // �¾�
float4 LightDiffuse; // ���� ��
float4 MatrealDiffuse; // ��ü�� �޾Ƶ��̴� ���� ��
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

// Diffuse(�л걤)
// ��ü�� ǥ�鿡�� �л�Ǿ� ������ �ٷ� ������ ��
// ������ ���� ��Ⱑ �ٸ���(Lambert ����)

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