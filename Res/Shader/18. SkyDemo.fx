#include "00. Global.fx"
#include "00. Light.fx"

struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_OUT VS(PNTTVertex input)
{
    VS_OUT output;
	
    float4 viewPosition = mul(float4(input.position.xyz, 0), View);
    output.position = mul(viewPosition, Projection);

   // output.position.z = output.position.w; // rasterizer �ܰ迡�� w������ x,y,z�� ������ ������ z���� w������ �ٲ��� ������ 1�� ������ش�.
    output.position.z = output.position.w * 0.999999f; // z���� 1���� �۰� ���� z-fighting�� �����Ѵ�.
    
    output.uv = input.uv;
    
	return output;
}

float4 PS(VS_OUT input) : SV_TARGET
{
	float4 color = DiffuseMap.Sample(LinearSampler, input.uv);

	return color;
}

technique11 T0
{
	pass P0
    {
        SetRasterizerState(FrontCounterClockwise);
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
};
