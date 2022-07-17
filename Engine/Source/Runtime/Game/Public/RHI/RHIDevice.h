// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class RHIFactory;
class RHICommandQueue;
class RHISwapChain;
interface IPlatformWindow;

class GAME_API RHIDevice : public std::enable_shared_from_this<RHIDevice>
{
	std::shared_ptr<RHIFactory> _factory;

protected:
	RHIDevice(std::shared_ptr<RHIFactory> factory);

public:
	virtual ~RHIDevice() noexcept;

	std::shared_ptr<RHIFactory> GetFactory() const;

	virtual std::shared_ptr<RHICommandQueue> CreateCommandQueue() = 0;
	virtual std::shared_ptr<RHISwapChain> CreateSwapChain(std::shared_ptr<RHICommandQueue> queue, IPlatformWindow* drawingWindow) = 0;
};