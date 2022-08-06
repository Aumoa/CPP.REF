// Copyright 2020-2022 Aumoa.lib. All right reserved.

RWTexture2D<float4> gColorOutput : register(u0);

[shader("raygeneration")]
void RayGeneration()
{
	uint2 launchIndex = (uint2)DispatchRaysIndex();
	gColorOutput[launchIndex] = float4(1.0f, 1.0f, 1.0f, 1.0f);
}