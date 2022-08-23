// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHISwapChain.h"

class NullRHIDevice;

class NullRHISwapChain : public RHISwapChain
{
	friend NullRHIDevice;

private:
	std::atomic<size_t> _index;
	std::vector<std::shared_ptr<RHIResource>> _buffers;

private:
	NullRHISwapChain(std::shared_ptr<RHIDevice> device);

public:
	virtual ~NullRHISwapChain() noexcept override;

	virtual void Present() override;
	virtual void ResizeBuffers(const Vector2N& size) override;
	virtual size_t GetCurrentBackBufferIndex() const override;
	virtual std::shared_ptr<RHIResource> GetBuffer(size_t index) override;
};