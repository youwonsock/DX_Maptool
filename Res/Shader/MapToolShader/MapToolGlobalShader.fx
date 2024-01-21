#ifndef _MAP_TOOL_GLOBAL_FX_
#define _MAP_TOOL_GLOBAL_FX_

#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500
#define MAX_MODEL_INSTANCE 500

cbuffer GlobalBuffer
{
    matrix View;
    matrix Projection;
    matrix ViewProjection;
    matrix ViewInverse; // == cam world matrix
};

cbuffer TransformBuffer
{
    matrix World;
};

cbuffer ShadowBuffer
{
    matrix ShadowViewProjection;
};

/// Vertex Input ///

struct VertexModel
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 tangent : TANGENT;
    
    // skinning animation
    // float4 blendIndices : BLENDINDICES;
    // float4 blendWeights : BLENDWEIGHT;
    
    uint instanceID : SV_INSTANCEID;
    matrix world : INST;
};

struct PNCTVertex
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

/// Vertex Input ///



/// Vertex Output ///

struct MeshOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 eye : EYE;
    float3 lightDir : LIGHTDIR;
    
    float4 color : COLOR; // for depth
    float4 shadow : SHADOW;
    
    float4 worldPosition : POSITION1;
    
    //float3 tangent : TANGENT;
};

struct PNCTOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR; // for depth
    float2 uv : TEXCOORD;
    
    float4 shadow : SHADOW;
};

/// Vertex Output ///



/// SamplerState ///

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState PointSampler
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState ClampSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

/// Sampler State ///



/// Resterizer State ///

RasterizerState FillModeWireFrame
{
    FillMode = WireFrame;
};

RasterizerState FrontCounterClockwise
{
    FrontCounterClockwise = true;
};

/// Resterizer State ///



/// Macro ///

#define PASS_VP(name, vs, ps)                           \
pass name                                               \
{                                                       \
        SetVertexShader(CompileShader(vs_5_0, vs()));   \
        SetPixelShader(CompileShader(ps_5_0, ps()));    \
}                  

#define PASS_RS_VP(name,rs, vs, ps)                     \
pass name                                               \
{                                                       \
        SetRasterizerState(rs);                         \
        SetVertexShader(CompileShader(vs_5_0, vs()));   \
        SetPixelShader(CompileShader(ps_5_0, ps()));    \
}                                                       

/// Macro ///



/// Function ///

float3 GetCameraPosition()
{
    return ViewInverse._41_42_43;
}

#endif