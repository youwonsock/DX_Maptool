#include "MapToolGlobalShader.fx"

TextureCube cubeMapTexture;
SamplerState pointClamp;

struct PNTROutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 Reflect : TEXCOORD1;
};

PNTROutput VS(PNCTVertex input)
{
    PNTROutput output = (PNTROutput) 0;

    output.position = mul(input.position, World);
    output.position = mul(output.position, ViewProjection);
    
    output.normal = input.normal;
    output.uv = input.uv;
    output.Reflect = normalize(input.position.xyz);
    
    return output;
}

float4 PS(PNTROutput input) : SV_TARGET
{
    float4 fColor = float4(1, 1, 1, 1);
    fColor = cubeMapTexture.Sample(pointClamp, input.Reflect);
    return fColor;
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