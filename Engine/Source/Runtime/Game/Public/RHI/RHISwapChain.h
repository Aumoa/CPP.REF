// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceResource.h"

class RHIResource;

class GAME_API RHISwapChain : public RHIDeviceResource
{
protected:
	RHISwapChain(std::shared_ptr<RHIDevice> device);

public:
	virtual ~RHISwapChain() noexcept override;

	virtual void Present() = 0;
	virtual void ResizeBuffers(const Vector2N& size) = 0;
	virtual size_t GetCurrentBackBufferIndex() const = 0;
	virtual std::shared_ptr<RHIResource> GetBuffer(size_t index) = 0;
};