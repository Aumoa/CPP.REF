// Shader Type: library
#include "Common.hlsli"

[shader("raygeneration")]
void main()
{
    Payload payload;
    payload.ColorAndDistance = float4(0, 0, 0, 0);

    uint2 launchIndex = DispatchRaysIndex().xy;
    float2 dims = float2(DispatchRaysDimensions().xy);
    float2 d = (((launchIndex.xy + 0.5f) / dims.xy) * 2.0f - 1.0f);

    RayDesc ray;
    ray.Origin = float3(d.x, -d.y, 1);
    ray.Direction = float3(0, 0, -1);
    ray.TMin = 0;
    ray.TMax = 100000.0f;

    TraceRay(
		t_Scene,
		RAY_FLAG_NONE,
		0xFF,
		0,
		0,
		0,
		ray,
		payload
	);

    u_OutputTexture[launchIndex] = float4(payload.ColorAndDistance.xyz, 1.0f);
}