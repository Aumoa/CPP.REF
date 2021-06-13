float4 Main(float4 posH : SV_Position, float3 color : COLOR) : SV_TARGET
{
    return float4(color, 1.0f);

}