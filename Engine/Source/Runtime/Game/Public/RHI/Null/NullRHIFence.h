// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIFence.h"
#include "Threading/Tasks/TaskCompletionSource.h"

class NullRHIDevice;
class NullRHICommandQueue;

class GAME_API NullRHIFence : public RHIFence
{
	friend NullRHIDevice;
	friend NullRHICommandQueue;

private:
	Spinlock _lock;
	uint64 _fenceValue = 0;
	std::map<uint64, TaskCompletionSource<>> _events;

private:
	NullRHIFence(std::shared_ptr<RHIDevice> device);

private:
	void SetFenceValue(uint64 fenceValue);

public:
	virtual ~NullRHIFence() noexcept override;

	virtual uint64 GetCompletedValue() const override;
	virtual void Wait(uint64 fenceValue) override;
};