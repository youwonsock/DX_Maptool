#ifndef _GLOBAL_FX_
#define _GLOBAL_FX_

// 모든 쉐이더에서 사용하는 공통된 데이터들을 정의한 .fx파일 

/// Constant Buffer ///

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

/// Constant Buffer ///



/// Vertex Input ///

struct PVertex
{
    float4 position : POSITION;
};

struct PTVertex
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct PCVertex
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PNTVertex
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct PNTTVertex
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 tangent : TANGENT;
};

struct PNCTVertex
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct PNTTBBVertex
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 tangent : TANGENT;
    float4 blendIndices : BLEND_INDICES;
    float4 blendWeights : BLEND_WEIGHT;
};


/// Vertex Input ///



/// Vertex Output ///

struct PNTOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

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

/// Function ///

#endif