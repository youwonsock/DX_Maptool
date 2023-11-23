matrix World;
matrix View;
matrix Projection;
Texture2D Texture0;
uint Address;

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

RasterizerState FillModeWireFrame
{
	FillMode = wireframe;
};

// filter = 확대/ 축소가 일어났을때 중간값을 처리하는 방식
// address = UI가 1보다 크거나 0보다 작을때 나머지 부분의 처리 방식

SamplerState Sampler0;

SamplerState SamplerAddressWrap
{
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState SamplerAddressMirror
{
    AddressU = Mirror;
    AddressV = Mirror;
};

SamplerState SamplerAddressClamp
{
    AddressU = Clamp;
    AddressV = Clamp;
};

SamplerState SamplerAddressBorder
{
    AddressU = Border;
    AddressV = Border;
    BorderColor = float4(1, 0, 0, 1);
};

float4 PS(VertexOutput input) : SV_TARGET
{
    if (Address == 0)
        return Texture0.Sample(SamplerAddressWrap, input.UV);
    if (Address == 1)
        return Texture0.Sample(SamplerAddressMirror, input.UV);
    if (Address == 2)
        return Texture0.Sample(SamplerAddressClamp, input.UV);
    if (Address == 3)
        return Texture0.Sample(SamplerAddressBorder, input.UV);
	
    return Texture0.Sample(Sampler0, input.UV);
}

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
};