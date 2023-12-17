#include "00. Global.fx"
#include "00. Light.fx"

#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500
#define MAX_MODEL_INSTANCE 500

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

struct TweenFrameDesc
{
    float tweenDuration;
    float tweenRatio;
    float tweenSumTime;
    float padding;
    KeyframeDesc curr;
    KeyframeDesc next;
};

cbuffer BoneBuffer
{
    matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
};

cbuffer InstancedTweenBuffer
{
    TweenFrameDesc TweenFrames[MAX_MODEL_INSTANCE];
};

uint BoneIndex;
Texture2DArray TransformMap;

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

matrix GetAnimationWorldMatrix(VS_IN input)
{
    float indices[4] = { input.blendIndices.x, input.blendIndices.y, input.blendIndices.z, input.blendIndices.w };
    float weights[4] = { input.blendWeights.x, input.blendWeights.y, input.blendWeights.z, input.blendWeights.w };

    int animIndex[2];
    int currentFrame[2];
    int nextFrame[2];
    float ratio[2];
    
    animIndex[0] = TweenFrames[input.instanceID].curr.animIndex;
    currentFrame[0] = TweenFrames[input.instanceID].curr.currentFrame;
    nextFrame[0] = TweenFrames[input.instanceID].curr.nextFrame;
    ratio[0] = TweenFrames[input.instanceID].curr.ratio;
    
    animIndex[1] = TweenFrames[input.instanceID].next.animIndex;
    currentFrame[1] = TweenFrames[input.instanceID].next.currentFrame;
    nextFrame[1] = TweenFrames[input.instanceID].next.nextFrame;
    ratio[1] = TweenFrames[input.instanceID].next.ratio;
    
    float4 c0, c1, c2, c3; // current
    float4 n0, n1, n2, n3; // next
    matrix cur = 0;
    matrix transform = 0;
    matrix next = 0;
	
    for (int i = 0; i < 4; ++i)
    {
        c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currentFrame[0], animIndex[0], 0));
        c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currentFrame[0], animIndex[0], 0));
        c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currentFrame[0], animIndex[0], 0));
        c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currentFrame[0], animIndex[0], 0));
        cur = matrix(c0, c1, c2, c3);
		
        n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
        n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
        n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
        n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
        next = matrix(n0, n1, n2, n3);
		
        matrix result = lerp(cur, next, ratio[0]);
        
        // 다음 애니메이션
        if (animIndex[1] >= 0)
        {
            c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currentFrame[1], animIndex[1], 0));
            c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currentFrame[1], animIndex[1], 0));
            c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currentFrame[1], animIndex[1], 0));
            c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currentFrame[1], animIndex[1], 0));
            cur = matrix(c0, c1, c2, c3);
		
            n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
            n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
            n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
            n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));
            next = matrix(n0, n1, n2, n3);
		
            matrix nextResult = lerp(cur, next, ratio[1]);
            result = lerp(result, nextResult, TweenFrames[input.instanceID].tweenRatio);
        }
        
        transform += mul(weights[i], result);
    }
	
    return transform;
}

VS_OUT VS(VS_IN input)
{
    VS_OUT output;
	
    matrix m = GetAnimationWorldMatrix(input);
    
    output.position = mul(input.position, m);
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
