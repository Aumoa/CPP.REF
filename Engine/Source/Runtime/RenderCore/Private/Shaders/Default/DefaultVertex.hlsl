// Shader Type: vertex

#define __VERTEX_SHADER__

#include "DefaultCommon.hlsli"

Fragment main(in Vertex vertex)
{
    Fragment f;
    f.Position = mul(float4(vertex.Position, 1), ViewProjection);
    f.Color = vertex.Color;
    return f;
}