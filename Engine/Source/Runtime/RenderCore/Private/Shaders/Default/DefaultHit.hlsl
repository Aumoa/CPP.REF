// Shader Type: library

#include "DefaultCommon.hlsli"

[shader("closesthit")]
void main(inout Payload payload, Attributes attr)
{
    payload.ColorAndDistance = float4(1, 1, 1, RayTCurrent());
}