// Shader Type: vertex

#include "DefaultCommon.hlsli"

Fragment main(in Vertex vertex)
{
    Fragment f;
    f.Position = float4(vertex.Position, 1);
    f.Color = vertex.Color;
    return f;
}