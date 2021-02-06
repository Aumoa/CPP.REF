// Copyright 2020 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"
#include "../GenericLighting.hlsli"

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

	Light myLight;
	myLight.Type = LIGHT_TYPE_DIRECTIONAL;
	myLight.Color = 1.0f;
	myLight.Ambient = 1.0f;
	myLight.Diffuse = 1.0f;
	myLight.Specular = 1.0f;
	myLight.Ambiguous_01 = float4(normalize(float3(0, 0.25f, -1.0f)), 0);

	Material myMat;
	myMat.Ambient = 0.2f;
	myMat.Diffuse = 0.5f;
	myMat.Specular = 0.2f;
	myMat.SpecExp = 16.0f;

	float3 ray = gCamera.Pos - worldPos;
	float3 rayDir = normalize(ray);
	float3 ads = ComputeDirectionalLight(myMat, myLight, gNormalBuffer.Sample(gSampler, inFrag.Tex).xyz, rayDir);

	HDRPixel oPixel;
	oPixel.Color = float4(float3((ads.x + ads.y) * myLight.Color.xyz + ads.z), 1.0f);
	return oPixel;
}