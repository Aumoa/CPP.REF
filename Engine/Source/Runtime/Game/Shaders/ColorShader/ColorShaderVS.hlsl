static float4 gPos[4] =
{
    float4(-1.0f, 1.0f, 0, 1.0f),
    float4(1.0f, 1.0f, 0, 1.0f),
    float4(-1.0f, -1.0f, 0, 1.0f),
    float4(1.0f, -1.0f, 0, 1.0f),
};

static float3 gColor[4] =
{
    float3(1.0f, 0.0f, 0.0f),
    float3(0.0f, 1.0f, 0.0f),
    float3(0.0f, 0.0f, 1.0f),
    float3(1.0f, 1.0f, 0.0f),
};

void Main(uint vertexId : SV_VertexID, out float4 posH : SV_Position, out float3 color : COLOR)
{
    posH = gPos[vertexId];
    color = gColor[vertexId];
}