matrix World;
matrix View;
matrix Projection;

struct PNCTVertex
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct PNCTOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

PNCTOutput VS(PNCTVertex input)
{
    PNCTOutput output;
    output.position = input.position;
    output.position = mul(output.position, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
	
    output.color = input.color;
    output.normal = input.normal;
    output.uv = input.uv;
	
    return output;
}

RasterizerState FillModeWireFrame
{
    FillMode = wireframe;
};

float4 PS(PNCTOutput input) : SV_TARGET
{
    return input.color;
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