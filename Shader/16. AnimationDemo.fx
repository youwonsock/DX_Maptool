#include "00. Global.fx"
#include "00. Light.fx"

#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500

struct KeyframeDesc
{
    uint animIndex;
    uint currentFrame;
	uint nextFrame;
    float ratio;
    float sumTime;
    float speed;
    float2  padding;
};

cbuffer BoneBuffer
{
	matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
};

cbuffer KeyframeBuffer
{
	KeyframeDesc Keyframes;
};

uint BoneIndex;
Texture2DArray TransformMap;

/* not use lerp
matrix GetAnimationWorldMatrix(PNTTBBVertex input)
{
    float indices[4] = { input.blendIndices.x, input.blendIndices.y, input.blendIndices.z, input.blendIndices.w };
    float weights[4] = { input.blendWeights.x, input.blendWeights.y, input.blendWeights.z, input.blendWeights.w };

	int animIndex = Keyframes.animIndex;
	int currentFrame = Keyframes.currentFrame;
	
    float4 c0, c1, c2, c3;
    matrix cur, transform;
	
    for (int i = 0; i < 4; ++i)
    {
		c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currentFrame, animIndex, 0));
		c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currentFrame, animIndex, 0));
		c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currentFrame, animIndex, 0));
		c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currentFrame, animIndex, 0));
		
		cur = matrix(c0, c1, c2, c3);
        transform += mul(weights[i], cur);
    }
	
	return transform;
}
*/

// use lerp
matrix GetAnimationWorldMatrix(PNTTBBVertex input)
{
    float indices[4] = { input.blendIndices.x, input.blendIndices.y, input.blendIndices.z, input.blendIndices.w };
    float weights[4] = { input.blendWeights.x, input.blendWeights.y, input.blendWeights.z, input.blendWeights.w };

    int animIndex = Keyframes.animIndex;
    int currentFrame = Keyframes.currentFrame;
	int nextFrame = Keyframes.nextFrame;
	
    float4 c0, c1, c2, c3;	// current
    float4 n0, n1, n2, n3;	// next
    matrix cur, transform, next;
	
    for (int i = 0; i < 4; ++i)
    {
        c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currentFrame, animIndex, 0));
        c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currentFrame, animIndex, 0));
        c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currentFrame, animIndex, 0));
        c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currentFrame, animIndex, 0));
        cur = matrix(c0, c1, c2, c3);
		
        n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame, animIndex, 0));
        n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame, animIndex, 0));
        n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame, animIndex, 0));
        n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame, animIndex, 0));
        next = matrix(n0, n1, n2, n3);
		
		matrix result = lerp(cur, next, Keyframes.ratio);
        transform += mul(weights[i], cur);
    }
	
    return transform;
}

MeshOutput VS(PNTTBBVertex input)
{
	MeshOutput output;
	
	// todo 
    matrix m = GetAnimationWorldMatrix(input);
	
	output.position = mul(input.position, m);
	output.position = mul(output.position, World);
	output.worldPosition = output.position.xyz;
	output.position = mul(output.position, ViewProjection);	
	output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)World);
	output.tangent = mul(input.tangent, (float3x3)World);

	return output;
}

float4 PS(MeshOutput input) : SV_TARGET
{
	//ComputeNormalMapping(input.normal, input.tangent, input.uv);
	//float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);
	float4 color = DiffuseMap.Sample(LinearSampler, input.uv);

	return color;
}

float4 PS_RED(MeshOutput input) : SV_TARGET
{
	return float4(1, 0, 0, 1);
}

technique11 T0
{
	PASS_VP(P0, VS, PS)
	PASS_RS_VP(P1, FillModeWireFrame, VS, PS_RED)
};
