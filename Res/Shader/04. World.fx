matrix World;
matrix View;
matrix Projection;
Texture2D Texture0;

struct VertexInput
{
	float4 position : POSITION;
    float2 UV : TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 UV : TEXCOORD;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	output.position = input.position;
	output.position = mul(output.position, World);
	output.position = mul(output.position, View);
	output.position = mul(output.position, Projection);
	
    output.UV = input.UV;
	
	return output;
}

SamplerState Sampler0;
float4 PS(VertexOutput input) : SV_TARGET
{
    return Texture0.Sample(Sampler0, input.UV);
}

RasterizerState FillModeWireFrame
{
	FillMode = wireframe;
};

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

	pass P1
	{
        SetRasterizerState(FillModeWireFrame);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};