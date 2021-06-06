static float4 gPos[4] =
{
    float4(-1.0f, 1.0f, 0, 1.0f),
    float4(1.0f, 1.0f, 0, 1.0f),
    float4(1.0f, -1.0f, 0, 1.0f),
    float4(-1.0f, -1.0f, 0, 1.0f),
};

float4 Main(uint vertexId : SV_VertexID) : SV_POSITION
{
    return gPos[vertexId];
}