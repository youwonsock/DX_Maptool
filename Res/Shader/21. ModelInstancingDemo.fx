#include "00. Global.fx"
#include "00. Light.fx"

#define MAX_MODEL_TRANSFORMS 250

cbuffer BoneBuffer
{
    matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
};
uint BoneIndex;

struct VS_IN
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 tangent : TANGENT;
    
    float4 blendIndices : BLENDINDICES;
    float4 blendWeights : BLENDWEIGHT;
    
    // instancing
    uint instanceID : SV_INSTANCEID;
    matrix world : INST;    // shader class 미리 정의된 semantic name
}; 

struct VS_OUT
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION1;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

VS_OUT VS(VS_IN input)
{
    VS_OUT output;
	
    output.position = mul(input.position, BoneTransforms[BoneIndex]);
    output.position = mul(output.position, input.world);
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, ViewProjection);
    output.uv = input.uv;
    output.normal = input.normal;
    
	return output;
}

float4 PS(VS_OUT input) : SV_TARGET
{
	float4 color = DiffuseMap.Sample(LinearSampler, input.uv);

	return color;
}

technique11 T0
{
    PASS_VP(P0, VS, PS)
};
