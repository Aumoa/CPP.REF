struct CameraConstant
{
    row_major matrix World;
    row_major matrix WorldInvTranspose;
    float3 CameraPos;
};

struct VertexElement
{
    float3 Pos : POSITION;
    float3 Color : COLOR;
};

ConstantBuffer<CameraConstant> gCamera : register(b0);

void Main(VertexElement element, out float4 posH : SV_Position, out float3 color : COLOR)
{
    posH = float4(element.Pos, 1.0f);
    color = element.Color;
}