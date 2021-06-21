cbuffer C1 : register(b1)
{
    float gColor;
};

float4 Main(float4 posH : SV_Position, float3 normal : NORMAL, float3 color : COLOR) : SV_TARGET
{
    float light = dot(normal, normalize(float3(0, 1.0f, -0.5f)));
    return float4((float3)gColor * light, 1.0f);
}