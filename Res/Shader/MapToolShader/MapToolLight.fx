#ifndef _MAP_TOOL_GLOBAL_LIGHT_FX_
#define _MAP_TOOL_GLOBAL_LIGHT_FX_\

#include "MapToolGlobalShader.fx"

Texture2D DiffuseMap;
Texture2D SpecularMap;
Texture2D NormalMap;

/// Struct ///

struct LightDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    float3 direction;
    float Padding;
};

struct MatrealDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
};

/// Struct ///


/// constant buffer ///

cbuffer LightBuffer
{
    LightDesc GlobalLight;
};

cbuffer MaterialBuffer
{
    MatrealDesc Matreal;
};

/// constant buffer ///


float4 ComputeLight(float3 normal, float2 uv, float3 eye, float3 lightDirection)
{
    float4 ambientColor = 0;
    float4 diffuseColor = 0;
    float4 specularColor = 0;
    float4 emissiveColor = 0;
    
    // ambient
    {
        float4 color = GlobalLight.ambient * Matreal.ambient;
        ambientColor = DiffuseMap.Sample(LinearSampler, uv) * color;
    }
    
    //diffuse
    {
        float4 color = DiffuseMap.Sample(LinearSampler, uv);
        float val = dot(normalize(normal), -lightDirection);
        diffuseColor = color * val * Matreal.diffuse * GlobalLight.diffuse;
    }
    
    //specular
    {
        float3 R = reflect(lightDirection, normal);
        R = normalize(R);
        
        float val = saturate(dot(eye, R));
        float specular = pow(val, 10);

        specularColor = GlobalLight.specular * Matreal.specular * specular;
    }
    
    //emissive
    {
        float val = saturate(dot(eye, normal));
        float emissive = 1.0f - val;
        
        emissive = smoothstep(0.0f, 1.0f, emissive);
        emissive = pow(emissive, 2.0f);

        emissiveColor = GlobalLight.emissive * Matreal.emissive * emissive;
    }
    
    return ambientColor + diffuseColor + specularColor + emissiveColor;
}

float3x3 ComputeTBN(float3 normal, float3 tangent)
{
    float3 Normal = normalize(normal); 
    float3 Tangent = normalize(tangent);
    float3 Binormal = normalize(cross(Normal, Tangent)); 
    float3x3 TBN = float3x3(Tangent, Binormal, Normal); 
    
    return transpose(TBN);
}

/// Function ///

#endif