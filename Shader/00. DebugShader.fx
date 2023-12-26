#include "00. Global.fx"
#include "00. Light.fx"
#include "00. Render.fx"

struct PNCTVertex
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
    
};

struct PNCTOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
    
};

PNCTOutput VS(PNCTVertex input)
{
    PNCTOutput output;
    output.position = input.position;
    output.position = mul(output.position, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
	
    output.normal = input.normal;
    output.uv = input.uv;
	
    return output;
}

Texture2D g_txDiffuse1 : register(t0);
SamplerState sample0 : register(s0);

float4 PS(PNCTOutput input) : SV_TARGET
{
    return g_txDiffuse1.Sample(sample0, input.uv);// * input.color;
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