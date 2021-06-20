float4 Main(float4 posH : SV_Position, float3 normal : NORMAL, float3 color : COLOR) : SV_TARGET
{
    float light = dot(normal, float3(0, 0, -1));
    return float4(color * light, 1.0f);
}