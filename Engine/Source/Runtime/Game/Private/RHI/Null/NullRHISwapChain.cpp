// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHISwapChain.h"
#include "RHI/Null/NullRHIDevice.h"
#include "RHI/RHIStructures.h"

NullRHISwapChain::NullRHISwapChain(std::shared_ptr<RHIDevice> device)
	: RHISwapChain(std::move(device))
{
	for (size_t i = 0; i < 3; ++i)
	{
		_buffers[i] = device->CreateCommittedResource(RHIHeapProperties(), ERHIHeapFlags::None, RHIResourceDesc(), ERHIResourceStates::Common, nullptr);
	}
}

NullRHISwapChain::~NullRHISwapChain() noexcept
{
}

void NullRHISwapChain::Present()
{
	++_index;
}

void NullRHISwapChain::ResizeBuffers(const Vector2N& size)
{
	check(size.X != 0 && size.Y != 0);
}

size_t NullRHISwapChain::GetCurrentBackBufferIndex() const
{
	return _index.load() % 3;
}

std::shared_ptr<RHIResource> NullRHISwapChain::GetBuffer(size_t index)
{
	return _buffers[index];
}