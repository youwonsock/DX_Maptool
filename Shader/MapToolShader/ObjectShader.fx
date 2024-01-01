#include "MapToolGlobalShader.fx"
 
// instancing 적용 시 월드 행렬 변경 필요 (별도의 쉐이더 함수를 만들까?)

#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500
#define MAX_MODEL_INSTANCE 500

MeshOutput VS_Model(VertexModel input)
{
    MeshOutput output;
	
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

cbuffer InstancedKeyframeBuffer
{
    KeyframeDesc Keyframes[MAX_MODEL_INSTANCE];
};

uint BoneIndex;
Texture2DArray TransformMap;

matrix GetAnimationMatrix(VertexModel input)
{
    int animIndex = Keyframes[input.instanceID].animIndex;
    int currFrame = Keyframes[input.instanceID].currentFrame;
    
    float4 c0, c1, c2, c3;

    matrix curr = 0;
    
    c0 = TransformMap.Load(int4(BoneIndex * 4 + 0, currFrame, animIndex, 0));
    c1 = TransformMap.Load(int4(BoneIndex * 4 + 1, currFrame, animIndex, 0));
    c2 = TransformMap.Load(int4(BoneIndex * 4 + 2, currFrame, animIndex, 0));
    c3 = TransformMap.Load(int4(BoneIndex * 4 + 3, currFrame, animIndex, 0));
    
    curr = matrix(c0, c1, c2, c3);

    return curr;
}

MeshOutput VS_Animation(VertexModel input)
{
    MeshOutput output;
    
    matrix m = GetAnimationMatrix(input);
    output.position = mul(input.position, m);
    output.position = mul(output.position, input.world);
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