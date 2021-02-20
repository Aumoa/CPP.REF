// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"
#include "../GenericLighting.hlsli"
#include "../PrimitivePacking.hlsli"

Texture2D<float4> gColorBuffer : register(t0);
Texture2D<uint4> gNormalBuffer : register(t1);
Texture2D<float> gDepthBuffer : register(t2);

SamplerState gSampler : register(s0);

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

HDRPixel PS_Main(in QuadFrag inFrag)
{
	float4 color = gColorBuffer.Sample(gSampler, inFrag.Tex);
	float3 worldPos = WorldPosFromDepth(inFrag.Tex, gDepthBuffer.Sample(gSampler, inFrag.Tex), gCamera);
	uint4 normalRaw = gNormalBuffer[(uint2)inFrag.PosH.xy];

	float3 normal = ConvUInt16ToSnorm16(normalRaw.xyz);	
	uint matIndex = normalRaw.w;

	float3 ray = gCamera.Pos - worldPos;
	float3 rayDir = normalize(ray);
	float3 ads = ComputeDirectionalLight(gMaterialBuffer[matIndex], gLight, normal, rayDir);

	float ambient = ads.x;
	float diffuse = ads.y;
	float specular = ads.z;

	float3 finalColor = ((ambient + diffuse) * color.xyz + specular) * gLight.Color;

	HDRPixel oPixel;
	oPixel.Color = float4(finalColor, 1.0f);
	return oPixel;
}