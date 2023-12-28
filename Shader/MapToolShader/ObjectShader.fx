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

Texture2DArray TransformMap;

matrix GetAnimationMatrix(VertexModel input)
{
    float indices[4] = { input.blendIndices.x, input.blendIndices.y, input.blendIndices.z, input.blendIndices.w };
    float weights[4] = { input.blendWeights.x, input.blendWeights.y, input.blendWeights.z, input.blendWeights.w };

    int animIndex = Keyframes.animIndex;
    int currFrame = Keyframes.currentFrame;
    int nextFrame = Keyframes.nextFrame;
    float ratio = Keyframes.ratio;

    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;

    matrix curr = 0;
    matrix next = 0;
    matrix transform = 0;

    for (int i = 0; i < 4; i++)
    {
        c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame, animIndex, 0));
        c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame, animIndex, 0));
        c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame, animIndex, 0));
        c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame, animIndex, 0));
        curr = matrix(c0, c1, c2, c3);

        n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame, animIndex, 0));
        n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame, animIndex, 0));
        n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame, animIndex, 0));
        n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame, animIndex, 0));
        next = matrix(n0, n1, n2, n3);

        matrix result = lerp(curr, next, ratio);

        transform += mul(weights[i], result);
    }

    return transform;
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