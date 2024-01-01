#include "MapToolGlobalShader.fx"

matrix WorldD;
matrix ViewD;
matrix ProjectionD;

PNCTOutput VS(PNCTVertex input)
{
    PNCTOutput output;
    output.position = input.position;
    output.position = mul(output.position, ViewD);
    output.position = mul(output.position, ProjectionD);
	
    output.color = input.color;
    output.normal = input.normal;
    output.uv = input.uv;
	
    return output;
}


float4 PS(PNCTOutput input) : SV_TARGET
{
    return float4(1,0,0,1);
}

technique11 T0
{        
    pass P0                                               
    {                                            
        SetRasterizerState(FillModeWireFrame);
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }                                                       
};