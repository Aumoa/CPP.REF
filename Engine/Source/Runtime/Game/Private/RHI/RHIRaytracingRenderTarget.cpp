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
	rt.UAV = device->CreateShaderResourceViewTable(ERHIShaderResourceViewType::CBV_SRV_UAV, 1);

	RHIUnorderedAccessViewDesc uav =
	{
		.Format = ERHIPixelFormat::B8G8R8A8_UNORM,
		.ViewDimension = ERHIUAVDimension::Texture2D,
		.Texture2D =
		{
			.MipSlice = 0,
			.PlaneSlice = 0
		}
	};

	rt.UAV->CreateUnorderedAccessView(0, rt.ColorTarget, nullptr, uav);
	return rt;
}