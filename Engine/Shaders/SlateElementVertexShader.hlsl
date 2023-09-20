#include "SlateElementCommon.hlsli"

Fragment main(in Vertex InVertex)
{
    Fragment F;
    F.Position = float4(InVertex.Position, 0, 1.0f);
    return F;
}