#ifndef _GLOBAL_FX_
#define _GLOBAL_FX_

// 모든 쉐이더에서 사용하는 공통된 데이터들을 정의한 .fx파일 

/// Constant Buffer ///

cbuffer GlobalBuffer
{
    matrix View;
    matrix Projection;
    matrix ViewProjection;
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

/// Vertex Input ///



/// Vertex Output ///

struct PNTOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
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

RasterizerState FillMode
{
    FillMode = WireFrame;
};

/// Resterizer State ///



/// Macro ///

#define PASS_VP(name, vs, ps)                           \
pass name                                               \
{                                                       \
        SetVertexShader(CompileShader(vs_5_0, vs()));   \
        SetPixelShader(CompileShader(ps_5_0, ps()));    \
}                                                       

/// Macro ///



/// Function ///

/// Function ///

#endif