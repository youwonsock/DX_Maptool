
ByteAddressBuffer Input;    //SRV(RawBuffer)
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
    uint index = input.groupID.x * (10 * 8 * 3) + input.groupIndex;
    uint outAddress = index * 11 * 4; // 10 uints per group (ComputeInput), 4 bytes per uint (메모리 주소를 찾기 위한 연산)
    
    uint inAddress = index * 4; // index : 몇 번째 데이터인지, 4 : 데이터 하나의 크기
    float value = Input.Load(inAddress);
    
    Output.Store3(outAddress + 0, input.groupID);
    Output.Store3(outAddress + 12, input.groupThreadID);
    Output.Store3(outAddress + 24, input.dispatchThreadID);
    Output.Store(outAddress + 36, input.groupIndex);
    Output.Store(outAddress + 40, (uint)value);
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
