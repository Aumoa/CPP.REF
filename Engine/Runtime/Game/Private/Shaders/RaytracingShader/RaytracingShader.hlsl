// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"

ConstantBuffer<ShaderCameraConstant> gCamera : register(b0);
RaytracingAccelerationStructure gSceneAS : register(t0);

RWTexture2D<float4> gColorOutput : register(u0);

struct TraceArgs
{
	uint RayFlags;
	uint InstanceInclusionMask;
	uint RayContributionToHitGroupIndex;
	uint MultiplierForGeometryContributionToHitGroupIndex;
	uint MissShaderIndex;
	RayDesc Ray;
};

inline void TraceRayWithArgs(RaytracingAccelerationStructure inScene, TraceArgs inArgs, inout Payload payload)
{
	TraceRay(
		inScene,
		inArgs.RayFlags,
		inArgs.InstanceInclusionMask,
		inArgs.RayContributionToHitGroupIndex,
		inArgs.MultiplierForGeometryContributionToHitGroupIndex,
		inArgs.MissShaderIndex,
		inArgs.Ray,
		payload
	);
}

RayDesc GenerateRay(uint2 launchIndex, float3 cameraPos, matrix projectionToWorld, float near = 0.01f, float far = 1000.0f)
{
	float2 xy = launchIndex + 0.5f; // center in the middle of the pixel.
	float2 screenPos = xy / DispatchRaysDimensions().xy * 2.0 - 1.0;

	// Invert Y for DirectX-style coordinates.
	screenPos.y = -screenPos.y;

	// Unproject the pixel coordinate into a ray.
	float4 world = mul(float4(screenPos, 0, 1), projectionToWorld);
	world.xyz /= world.w;

	RayDesc ray;
	ray.Origin = cameraPos;
	ray.Direction = normalize(world.xyz - ray.Origin);
	ray.TMin = near;
	ray.TMax = far;

	//ray.Origin = float3(screenPos, -1.0f);
	//ray.Direction = gCamera.Pos;

	return ray;
}

[shader("raygeneration")]
void RayGeneration()
{
	uint2 launchIndex = (uint2)DispatchRaysIndex();

	TraceArgs args = (TraceArgs)0;
	args.RayFlags = RAY_FLAG_CULL_BACK_FACING_TRIANGLES;
	args.InstanceInclusionMask = ~0;
	args.MultiplierForGeometryContributionToHitGroupIndex = 1;
	args.Ray = GenerateRay(launchIndex, gCamera.Pos, gCamera.ViewProjInv);

	Payload payload;
	TraceRayWithArgs(gSceneAS, args, payload);

	gColorOutput[launchIndex] = payload.Color;
}

[shader("miss")]
void Miss(inout Payload payload : SV_Payload)
{
	payload.Color = 0;
}

StructuredBuffer<Vertex> cVertexBuffer : register(t0, space2);
StructuredBuffer<uint> cIndexBuffer : register(t1, space2);

[shader("closesthit")]
void ClosestHit(inout Payload payload : SV_Payload, BuiltInAttr attr)
{
	// 삼각형 보간 좌표를 조회합니다.
	float3 bary = float3(1.f - attr.Value.x - attr.Value.y, attr.Value.x, attr.Value.y);

	// 세 개의 정점을 찾습니다.
	Vertex triangles[3];

	uint primitiveID = PrimitiveIndex() * 3;
	uint triangleIndex[3] = { primitiveID + 0, primitiveID + 1, primitiveID + 2 };

	[unroll]
	for (int i = 0; i < 3; ++i)
	{
		triangleIndex[i] = cIndexBuffer[triangleIndex[i]];
		triangles[i] = cVertexBuffer[triangleIndex[i]];
	}

	// 보간된 조각 값을 찾습니다.
	RayFragment frag;
	frag.PosW = WorldRayOrigin() + RayTCurrent() * WorldRayDirection();
	frag.Tex = triangles[0].Tex * bary.x + triangles[1].Tex * bary.y + triangles[2].Tex * bary.z;
	//frag.NormalW = normalize(mul(triangles[0].NormalL * bary.x + triangles[1].NormalL * bary.y + triangles[2].NormalL * bary.z, (float3x3)lTransform.WorldInvTranspose));
	//frag.TangentW = normalize(mul(triangles[0].TangentL * bary.x + triangles[1].TangentL * bary.y + triangles[2].TangentL * bary.z, (float3x3)lTransform.World));

	payload.Color = 1.0f;
	payload.Pos = frag.PosW;
}