#include "MapToolGlobalShader.fx"

matrix WorldD;
matrix ViewD;
matrix ProjectionD;

PNCTOutput VS(PNCTVertex input)
{
    PNCTOutput output;
    output.position = mul(input.position, WorldD);
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
	PASS_VP(P0, VS, PS)
	PASS_RS_VP(P1, FillModeWireFrame, VS, PS)
};