float4 Main(float4 posH : SV_Position, float3 normal : NORMAL, float3 color : COLOR) : SV_TARGET
{
    float light = dot(normal, normalize(float3(0, 1.0f, -0.5f)));
    return float4(color * (1.0f - light * 0.2f), 1.0f);
}