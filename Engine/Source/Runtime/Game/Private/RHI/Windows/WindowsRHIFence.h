// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIFence.h"

#if PLATFORM_WINDOWS

#include "WindowsRHICommon.h"
#include "Threading/Tasks/TaskCompletionSource.h"
#include <set>

class WindowsRHIDevice;

class WindowsRHIFence : public RHIFence
{
	friend WindowsRHIDevice;

private:
	ComPtr<ID3D12Fence> _fence;

	Spinlock _lock;
	std::vector<HANDLE> _eventsPool;
	std::atomic<size_t> _eventsUsed;
	std::map<uint64, TaskCompletionSource<>> _tasks;

private:
	WindowsRHIFence(std::shared_ptr<WindowsRHIDevice> device);

public:
	virtual ~WindowsRHIFence() noexcept override;

	virtual uint64 GetCompletedValue() const override;
	virtual void Wait(uint64 fenceValue) override;

	inline ID3D12Fence* GetFence() const noexcept { return _fence.Get(); }
};

#endif