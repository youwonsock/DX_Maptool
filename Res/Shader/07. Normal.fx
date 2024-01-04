matrix World;
matrix View;
matrix Projection;
Texture2D Texture0;
float3 LightDir;

struct VertexInput
{
	float4 position : POSITION;
    float2 UV : TEXCOORD;
    float3 normal : NORMAL;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 UV : TEXCOORD;
    float3 normal : NORMAL;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	output.position = input.position;
	output.position = mul(output.position, World);
	output.position = mul(output.position, View);
	output.position = mul(output.position, Projection);
	
    output.UV = input.UV;
    output.normal = mul(input.normal, (float3x3)World);
    
	return output;
}

RasterizerState FillModeWireFrame
{
	FillMode = wireframe;
};

SamplerState Sampler0
{
    AddressU = Wrap;
    AddressV = Wrap;
};

float4 PS(VertexOutput input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    float3 light = -LightDir;
    
    return Texture0.Sample(Sampler0, input.UV) * dot(light, normal);
}

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