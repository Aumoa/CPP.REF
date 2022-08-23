// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIDevice.h"
#include "RHI/RHIFactory.h"
#include "RHI/RHIStructures.h"

RHIDevice::RHIDevice(std::shared_ptr<RHIFactory> factory)
	: _factory(std::move(factory))
{
}

RHIDevice::~RHIDevice() noexcept
{
}

std::shared_ptr<RHIFactory> RHIDevice::GetFactory() const
{
	return _factory;
}

std::shared_ptr<RHIResource> RHIDevice::CreateCommittedBuffer(ERHIHeapType heapType, uint64 bufferSize, ERHIResourceStates initialState, ERHIResourceFlags flags)
{
	RHIResourceDesc bufferDesc =
	{
		.Dimension = ERHIResourceDimension::Buffer,
		.Width = bufferSize,
		.Height = 1,
		.DepthOrArraySize = 1,
		.MipLevels = 1,
		.SampleDesc = { 1, 0 },
		.Layout = ERHITextureLayout::RowMajor,
		.Flags = flags
	};

	return CreateCommittedResource(RHIHeapProperties{ .Type = heapType }, ERHIHeapFlags::None, bufferDesc, initialState, nullptr);
}