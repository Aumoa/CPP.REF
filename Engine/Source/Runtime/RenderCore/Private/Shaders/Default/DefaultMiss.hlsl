// Shader Type: library

#include "DefaultCommon.hlsli"

[shader("miss")]
void DefaultMiss(inout Payload payload)
{
    uint2 launchIndex = DispatchRaysIndex().xy;
    float2 dims = float2(DispatchRaysDimensions().xy);
	
    float ramp = launchIndex.y / dims.y;
    payload.ColorAndDistance = float4(0, 0.2f, 0.7f - 0.3f * ramp, -1.0f);
}