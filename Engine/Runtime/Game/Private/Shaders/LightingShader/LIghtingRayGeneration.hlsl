// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"
#include "../GenericLighting.hlsli"
#include "../PrimitivePacking.hlsli"

RWTexture2D<float4> gColorOutput : register(u0);

Texture2D<float4> gColorBuffer : register(t0);
Texture2D<uint4> gNormalBuffer : register(t1);
Texture2D<float> gDepthBuffer : register(t2);

ConstantBuffer<ShaderCameraConstant> gCamera : register(b0);
ConstantBuffer<Light> gLight : register(b1);
StructuredBuffer<Material> gMaterialBuffer : register(t3);

float3 WorldPosFromDepth(float2 tex, float depth, ShaderCameraConstant cameraConstant)
{
	float z = depth;

	float4 clipSpacePosition = float4(tex * 2.0f - 1.0f, z, 1.0f);
	clipSpacePosition.y = -clipSpacePosition.y;

	float4 viewSpacePosition = mul(clipSpacePosition, cameraConstant.ViewProjInv);
	viewSpacePosition /= viewSpacePosition.w;

	return viewSpacePosition.xyz;
}

[shader("raygeneration")]
void LightingRayGeneration()
{
	uint2 launchIndex = (uint2)DispatchRaysIndex();
	uint2 dimensions = (uint2)DispatchRaysDimensions();

	float4 color = gColorBuffer[launchIndex];
	float3 worldPos = WorldPosFromDepth(launchIndex / (float2)dimensions, gDepthBuffer[launchIndex], gCamera);
	uint4 normalRaw = gNormalBuffer[launchIndex];

	float3 normal = ConvUInt16ToSnorm16(normalRaw.xyz);
	uint matIndex = normalRaw.w;

	float3 ray = gCamera.Pos - worldPos;
	float3 rayDir = normalize(ray);
	float3 ads = ComputeDirectionalLight(gMaterialBuffer[matIndex], gLight, normal, rayDir);

	float ambient = ads.x;
	float diffuse = ads.y;
	float specular = ads.z;

	float3 finalColor = ((ambient + diffuse) * color.xyz + specular) * gLight.Color;

	gColorOutput[launchIndex] = float4(finalColor, 1.0f);
}