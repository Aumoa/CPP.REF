// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"
#include "../GenericLighting.hlsli"

ConstantBuffer<ShaderCameraConstant> gCamera : register(b0);
RaytracingAccelerationStructure gSceneAS : register(t0);
StructuredBuffer<GeneralLight> gLights : register(t1);

RWTexture2D<float4> gColorOutput : register(u0);

inline float3 HitAttribute(float3 v1, float3 v2, float3 v3, BuiltInAttr bary)
{
	return v1 +
		bary.Value.x * (v2 - v1) +
		bary.Value.y * (v3 - v1);
}

inline float2 HitAttribute(float2 v1, float2 v2, float2 v3, BuiltInAttr bary)
{
	return v1 +
		bary.Value.x * (v2 - v1) +
		bary.Value.y * (v3 - v1);
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

	return ray;
}

[shader("raygeneration")]
void RayGeneration()
{
	uint2 launchIndex = (uint2)DispatchRaysIndex();

	Payload payload = (Payload)0;
	TraceRay(
		gSceneAS,
		RAY_FLAG_CULL_BACK_FACING_TRIANGLES | RAY_FLAG_FORCE_OPAQUE,
		0xFF,
		0,
		1,
		0,
		GenerateRay(launchIndex, gCamera.Pos, gCamera.ViewProjInv),
		payload
	);

	gColorOutput[launchIndex] = float4(payload.Color);
}

[shader("miss")]
void Miss(inout Payload payload : SV_Payload)
{
	payload.Color = 0;
}

StructuredBuffer<Vertex> cVertexBuffer : register(t0, space2);
StructuredBuffer<uint> cIndexBuffer : register(t1, space2);
ConstantBuffer<RaytracingInstanceTransform> cTransform : register(b0, space2);

[shader("closesthit")]
void ClosestHit(inout Payload payload : SV_Payload, BuiltInAttr attr)
{
	// Get primitive triangle.
	Vertex triangles[3];

	uint primitiveID = PrimitiveIndex() * 3;
	uint triangleIndex[3] = { primitiveID + 0, primitiveID + 1, primitiveID + 2 };

	[unroll]
	for (uint i = 0; i < 3; ++i)
	{
		triangleIndex[i] = cIndexBuffer[triangleIndex[i]];
		triangles[i] = cVertexBuffer[triangleIndex[i]];
	}

	// Interpolate fragment attributes.
	RayFragment frag;
	frag.PosW = WorldRayOrigin() + RayTCurrent() * WorldRayDirection();
	frag.Tex = HitAttribute(triangles[0].Tex, triangles[1].Tex, triangles[2].Tex, attr);
	frag.NormalW = normalize(mul(HitAttribute(triangles[0].Normal, triangles[1].Normal, triangles[2].Normal, attr), (float3x3)cTransform.WorldInvTranspose));
	//frag.TangentW = normalize(mul(triangles[0].TangentL * bary.x + triangles[1].TangentL * bary.y + triangles[2].TangentL * bary.z, (float3x3)cTransform.World));

	Material mat;
	mat.Ambient = 1.0f;
	mat.Diffuse = 1.0f;
	mat.Specular = 1.0f;
	mat.SpecExp = 32.0f;

	uint numLights;
	uint lightStride;
	gLights.GetDimensions(numLights, lightStride);

	float3 lights = 0;
	for (i = 0; i < numLights; ++i)
	{
		float3 ads = ComputeDirectionalLight(mat, gLights[i], frag.NormalW, -WorldRayDirection());
		lights += (ads.x + ads.y) * gLights[i].Color + ads.z;
	}

	payload.Color = float4(lights, 1.0f);
	payload.Pos = frag.PosW;
	payload.Normal = frag.NormalW;
}