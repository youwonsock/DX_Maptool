#include "Global.fx"

Texture2D Texture0;
Matrix view;
Matrix projection;

PNCTOutput VS(PNCTVertex input)
{
    PNCTOutput output;
    output.position = mul(float4(input.position.xyz, 0), view);
    output.position = mul(output.position, projection);
    output.position.z = output.position.w * 0.999999f;
    
    output.uv = input.uv;
	output.normal = input.normal;
	output.color = input.color;
	
	return output;
}

float4 PS(PNCTOutput input) : SV_TARGET
{
    return Texture0.Sample(LinearSampler, input.uv);
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