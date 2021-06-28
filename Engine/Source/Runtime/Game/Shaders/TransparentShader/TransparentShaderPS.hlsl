cbuffer C1 : register(b1)
{
    float gColor;
};

cbuffer C2 : register(b2)
{
    float gAlpha;
};

float4 Main(float4 posH : SV_Position, float3 normal : NORMAL, float3 color : COLOR) : SV_TARGET
{
    return float4(color * gColor, gAlpha);
}