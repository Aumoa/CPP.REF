// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIRaytracingRenderTarget.h"
#include "RHI/RHIStructures.h"

RHIRaytracingRenderTarget RHIRaytracingRenderTarget::Create(std::shared_ptr<RHIDevice> device, const Vector2N& textureSize)
{
	RHIHeapProperties heapProps =
	{
		.Type = ERHIHeapType::Default,
	};

	RHIResourceDesc desc =
	{
		.Dimension = ERHIResourceDimension::Texture2D,
		.Width = (uint64)textureSize.X,
		.Height = (uint32)textureSize.Y,
		.DepthOrArraySize = 1,
		.MipLevels = 1,
		.Format = ERHIPixelFormat::B8G8R8A8_UNORM,
		.SampleDesc = { 1, 0 },
		.Flags = ERHIResourceFlags::AllowUnorderedAccess,
	};

	RHIRaytracingRenderTarget rt;
	rt.Size = textureSize;
	rt.ColorTarget = device->CreateCommittedResource(heapProps, ERHIHeapFlags::None, desc, ERHIResourceStates::CopySource, nullptr);
	return rt;
}