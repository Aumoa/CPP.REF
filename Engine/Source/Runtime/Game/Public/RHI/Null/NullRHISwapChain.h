// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHISwapChain.h"

class NullRHIDevice;

class GAME_API NullRHISwapChain : public RHISwapChain
{
	friend NullRHIDevice;

private:
	NullRHISwapChain(std::shared_ptr<RHIDevice> device);

public:
	virtual ~NullRHISwapChain() noexcept override;

	virtual void Present() override;
	virtual void ResizeBuffers(const Vector2N& size) override;
};