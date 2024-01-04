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

   // output.position.z = output.position.w; // rasterizer 단계에서 w값으로 x,y,z를 나누기 때문에 z값을 w값으로 바꿔줘 무조건 1로 만들어준다.
    output.position.z = output.position.w * 0.999999f; // z값을 1보다 작게 만들어서 z-fighting을 방지한다.
    
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
