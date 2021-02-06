// Copyright 2020 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"

Texture2D<float4> gColorBuffer : register(t0);
Texture2D<float4> gNormalBuffer : register(t1);
Texture2D<float> gDepthBuffer : register(t2);

SamplerState gSampler : register(s0);

ConstantBuffer<ShaderCameraConstant> gCamera : register(b0);

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

	HDRPixel oPixel;
	oPixel.Color = float4(worldPos, 1.0f);
	return oPixel;
}