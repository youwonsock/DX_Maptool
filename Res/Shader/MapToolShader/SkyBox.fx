#include "MapToolLight.fx"

TextureCube CubeMapTexture;

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
    
    float4 viewPosition = mul(float4(input.position.xyz, 0), View);
    output.position = mul(viewPosition, Projection);
    output.position.z = output.position.w * 0.999999f;
    
    output.normal = input.normal;
    output.uv = input.uv;
    output.Reflect = normalize(input.position.xyz);
    
    return output;
}

float4 PS(PNTROutput input) : SV_TARGET
{
    float4 fColor = CubeMapTexture.Sample(PointSampler, input.Reflect);
    return fColor;
}


struct PNFROutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 Refraction : TEXCOORD0; // ±¼Àý
    float3 Reflect : TEXCOORD1;    // ¹Ý»ç
};

PNFROutput Object_VS(PNCTVertex input)
{
    PNFROutput output = (PNFROutput) 0;
    
    float4 vWorldPos = mul(input.position, World);
    float4 vViewPos = mul(vWorldPos, View);
    output.position = mul(vViewPos, Projection);
    float3 Incident = normalize(vWorldPos.xyz - GetCameraPosition());
    float3 vNormal = normalize(mul(input.normal, (float3x3) World));
    
    output.normal = vNormal;
    output.Reflect = normalize(reflect(Incident, vNormal));
    output.Refraction = normalize(Refraction(Incident, vNormal, 1.33f));
    
    return output;
}

float4 Object_PS(PNFROutput input) : SV_TARGET
{
    float4 ReflectedColor = CubeMapTexture.Sample(ClampSampler, input.Reflect);
    float4 RefractedColor = CubeMapTexture.Sample(ClampSampler, input.Refraction);
    
    float R0 = pow(1.0 - 1.33f, 2.0) / pow(1.0 + 1.33f, 2.0);
    float fresnel = ComputeFresnel_Map(input.Reflect, input.normal, R0);
    
    float4 color = lerp(RefractedColor, ReflectedColor, fresnel * 2);
    color.a = 1.f;
    
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

    pass P1
    {
       // SetRasterizerState(FrontCounterClockwise);
        SetVertexShader(CompileShader(vs_5_0, Object_VS()));
        SetPixelShader(CompileShader(ps_5_0, Object_PS()));
    }
}; 