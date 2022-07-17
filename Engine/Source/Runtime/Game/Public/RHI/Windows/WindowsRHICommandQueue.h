// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHICommandQueue.h"

#if PLATFORM_WINDOWS

#include "WindowsRHICommon.h"

class WindowsRHIDevice;

class GAME_API WindowsRHICommandQueue : public RHICommandQueue
{
	friend WindowsRHIDevice;

private:
	ComPtr<ID3D12CommandQueue> _queue;

private:
	WindowsRHICommandQueue(std::shared_ptr<WindowsRHIDevice> device);

public:
	virtual ~WindowsRHICommandQueue() noexcept override;

	inline ID3D12CommandQueue* GetQueue() const { return _queue.Get(); }
};

#endif