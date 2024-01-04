#ifndef _LIGHT_FX_
#define _LIGHT_FX_

#include "00. Global.fx"

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



/// SRV ///

Texture2D DiffuseMap;
Texture2D SpecularMap;
Texture2D NormalMap;

/// SRV ///



/// Function ///

float4 ComputeLight(float3 normal, float2 uv, float3 worldPostion)
{
    float4 ambientColor = 0;
    float4 diffuseColor = 0;
    float4 specularColor = 0;
    float4 emissiveColor = 0;
    
    float3 cameraPos = GetCameraPosition();
    float3 eye = normalize(cameraPos - worldPostion);
    
    // ambient
    {
        float4 color = GlobalLight.ambient * Matreal.ambient;
        ambientColor = DiffuseMap.Sample(LinearSampler, uv) * color;
    }
    
    //diffuse
    {
        float4 color = DiffuseMap.Sample(LinearSampler, uv);
        float val = dot(normalize(normal), -GlobalLight.direction);
        diffuseColor = color * val * Matreal.diffuse * GlobalLight.diffuse;
    }
    
    //specular
    {
        float3 R = reflect(GlobalLight.direction, normal);
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

// normal mapping       // inout : &
void ComputeNormalMapping(inout float3 normal, float3 tangent, float2 uv)
{
    float4 map = NormalMap.Sample(LinearSampler, uv);   // [0, 255] 범위에서 [0, 1] 범위로 변환
    
    if(any(map.rgb) == false)
        return;
    
    float3 Normal = normalize(normal);      // z
    float3 Tangent = normalize(tangent);    // x
    float3 Binormal = normalize(cross(Normal, Tangent));  // y
    float3x3 TBN = float3x3(Tangent, Binormal, Normal);   // tangent space -> world space 변환 행렬
    
    // [0, 1] 범위에서 [-1, 1] 범위로 변환
    float3 tangentSpaceNormal = (map.rgb * 2.0f) - 1.0f;
    
    // tangent space -> world space
    normal = mul(tangentSpaceNormal, TBN);
}

/// Function ///

#endif