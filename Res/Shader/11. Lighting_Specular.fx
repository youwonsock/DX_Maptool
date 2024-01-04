#include "00. Global.fx"

float3 LightDir;     // �¾�
float4 LightSpecular; // ���� ��
float4 MatrealSpecular; // ��ü�� �޾Ƶ��̴� ���� ��

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

// Specular (�ݻ籤)
// �ѹ������� ������ �ݻ�Ǵ� ��
float4 PS(MeshOutput input) : SV_TARGET
{
    // float3 R = reflect(-LightDir, normalize(input.normal)); // ���� �Լ��� �̿��� �ݻ� ����
    float3 R = LightDir - (2 * input.normal * dot(LightDir, input.normal));
    R = normalize(R);
    float3 cameraPos = -View._41_42_43;
    float3 eye = normalize(cameraPos - input.worldPosition);
    
    float val = saturate(dot(R, eye)); // clamp(0, 1);
    float specular = pow(val, 10);     // �ݻ籤�� ������ �����ϱ� ���� ���(���� ũ�� �ݻ�Ǵ� ���� ������ �۾���)
    
    float4 color = LightSpecular * MatrealSpecular * specular;
    
    return color;
}

technique11 T0
{
    PASS_VP(p0, VS, PS)
};