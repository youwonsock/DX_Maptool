
Texture2DArray<float4> Input;
RWTexture2DArray<float4> Output;

[numthreads(32, 32, 1)]
void CS(uint3 id : SV_DispatchThreadID)
{
    float4 color = Input.Load(int4(id, 0));
    
    Output[id] = float4(color.r, 1, 0, 1);
}

technique11 T0
{
    pass P0
    {
        SetVertexShader( NULL );
        SetPixelShader( NULL );
        SetComputeShader(CompileShader(cs_5_0, CS()));
    }
};
