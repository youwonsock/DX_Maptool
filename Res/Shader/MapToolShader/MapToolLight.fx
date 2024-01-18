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



/// Function ///

float ComputeFresnel_Map(float3 reflect, float3 normal, float F0)
{
    float cosAngle = 1 - saturate(dot(reflect, normal));
    float result = pow(cosAngle, 5.0f);
    result = saturate(mad(result, 1 - saturate(F0), F0));
    return result;
}

float ComputeFresnel_Light(float3 light, float3 normal, float F0)
{
    float cosAngle = 1 - saturate(dot(light, normal));
    float result = cosAngle * cosAngle;
    result = result * result;
    result = result * cosAngle;
    result = saturate(mad(result, 1 - saturate(F0), F0));
    return result;
}

float3 Refraction(float3 normal, float3 incident, float eta)
{
    float cosl = dot(normal, incident);
    float cosT2 = 1.0f - eta * eta * (1.0f - cosl * cosl);
    
    return eta * incident - (eta * cosl + sqrt(abs(cosT2))) * normal;
}

float3 GetHalfVector(float3 lightDirection, float3 viewDirection)
{
    return normalize(lightDirection + viewDirection);
}

float4 ComputeLight(float3 normal, float2 uv, float3 eye, float3 lightDirection)
{
    float4 ambientColor = 0;
    float4 diffuseColor = 0;
    float4 specularColor = 0;
    float4 emissiveColor = 0;
    
    float4 defaultColor = DiffuseMap.Sample(LinearSampler, uv);
    
    // ambient
    {
        ambientColor = GlobalLight.ambient * Matreal.ambient;
    }
    
    //diffuse
    {
        float val = dot(normalize(normal), -lightDirection);
        diffuseColor = val * Matreal.diffuse * GlobalLight.diffuse;
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
    
    float3 color = ambientColor + diffuseColor + specularColor + emissiveColor;
    return defaultColor * float4(color.xyz, 1.0);
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