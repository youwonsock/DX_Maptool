#ifndef _MAP_TOOL_GLOBAL_FX_
#define _MAP_TOOL_GLOBAL_FX_

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

/// Vertex Input ///

struct VertexModel
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 tangent : TANGENT;
    float4 blendIndices : BLENDINDICES;
    float4 blendWeights : BLENDWEIGHT;
    
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
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 worldPosition : POSITION1;
    float3 tangent : TANGENT;
};

struct PNCTOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
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