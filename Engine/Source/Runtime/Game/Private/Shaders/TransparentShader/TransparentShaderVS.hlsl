struct CameraConstant
{
    row_major matrix World;
    row_major matrix WorldViewProj;
    row_major matrix WorldInvTranspose;
};

struct VertexElement
{
    float3 Pos : POSITION;
    float3 Color : COLOR;
    float3 Normal : NORMAL;
};

ConstantBuffer<CameraConstant> gCamera : register(b0);

void Main(VertexElement element, out float4 posH : SV_Position, out float3 normal : NORMAL, out float3 color : COLOR)
{
    posH = mul(float4(element.Pos, 1.0f), gCamera.WorldViewProj);
    color = element.Color;
    normal = mul(element.Normal, (float3x3) gCamera.WorldInvTranspose);
}