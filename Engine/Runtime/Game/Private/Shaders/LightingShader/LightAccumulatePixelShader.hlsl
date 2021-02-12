// Copyright 2020 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"
#include "../GenericLighting.hlsli"
#include "../PrimitivePacking.hlsli"

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
	float3 worldPos = WorldPosFromDepth(inFrag.Tex, gDepthBuffer.Sample(gSampler, inFrag.Tex), gCamera);
	uint4 normalRaw = gNormalBuffer[(uint2)inFrag.PosH.xy];

	float3 normal = ConvUInt16ToSnorm16(normalRaw.xyz);	
	uint matIndex = normalRaw.w;

	float3 ray = gCamera.Pos - worldPos;
	float3 rayDir = normalize(ray);
	float3 ads = ComputeDirectionalLight(gMaterialBuffer[matIndex], gLight, normal, rayDir);

	HDRPixel oPixel;
	oPixel.Color = float4(float3((ads.x + ads.y) * gLight.Color + ads.z), 1.0f);
	return oPixel;
}