
RWByteAddressBuffer Output; //UAV(RawBuffer)

struct ComputeInput
{
    uint3 groupID : SV_GroupID;
    uint3 groupThreadID : SV_GroupThreadID;
    uint3 dispatchThreadID : SV_DispatchThreadID;
    uint groupIndex : SV_GroupIndex;
};

[numthreads(10, 8, 3)]
void CS(ComputeInput input)
{
    uint index = input.groupIndex;
    uint outAddress = index * 10 * 4; // 10 uints per group (ComputeInput), 4 bytes per uint (메모리 주소를 찾기 위한 연산)
    
    Output.Store3(outAddress + 0, input.groupID);
    Output.Store3(outAddress + 12, input.groupThreadID);
    Output.Store3(outAddress + 24, input.dispatchThreadID);
    Output.Store3(outAddress + 36, input.groupIndex);

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
