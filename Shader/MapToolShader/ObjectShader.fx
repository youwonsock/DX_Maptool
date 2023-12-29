#include "MapToolGlobalShader.fx"
 
#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500
#define MAX_MODEL_INSTANCE 500

cbuffer BoneBuffer
{
    matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
};
uint BoneIndex;

MeshOutput VS_Model(VertexModel input)
{
    MeshOutput output;
	
    output.position = mul(input.position, BoneTransforms[BoneIndex]); 
    output.position = mul(input.position, World);
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, ViewProjection);
    output.uv = input.uv;
    output.normal = input.normal;
    
    return output;
}

// ------------------------------------------------------------------------------
//----------------------------------anim-----------------------------------------
// ------------------------------------------------------------------------------

struct KeyframeDesc
{
    int animIndex;
    uint currentFrame;
    uint nextFrame;
    float ratio;
    float sumTime;
    float speed;
    float2 padding;
};

cbuffer KeyframeBuffer
{
    KeyframeDesc Keyframes;
};

matrix AnimMatrix;

MeshOutput VS_Animation(VertexModel input)
{
    MeshOutput output;
	
    matrix m = AnimMatrix;
    
    //output.position = mul(input.position, BoneTransforms[BoneIndex]);
    output.position = mul(input.position, m);

    output.position = mul(output.position, World);
    
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, ViewProjection);
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) input.world);
    output.tangent = mul(input.tangent, (float3x3) input.world);
    
    return output;
}


//----------------------------------------------------------------
//-------------------------------PS-------------------------------
//----------------------------------------------------------------

Texture2D DiffuseMap;

float4 PS(MeshOutput input) : SV_TARGET
{
    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);

    return color;
}


technique11 T0
{
    PASS_VP(P0, VS_Model, PS)
    PASS_VP(P1, VS_Animation, PS)
};