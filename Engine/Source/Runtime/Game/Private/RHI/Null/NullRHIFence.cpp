// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHIFence.h"

NullRHIFence::NullRHIFence(std::shared_ptr<RHIDevice> device)
	: RHIFence(std::move(device))
{
}

NullRHIFence::~NullRHIFence() noexcept
{
}

void NullRHIFence::SetFenceValue(uint64 value)
{
	std::unique_lock lock(_lock);
	_fenceValue = value;

	// Wakeup pending events.
	std::vector<TaskCompletionSource<>> completedEvents;
	bool eraseAll = true;
	for (auto it = _events.begin(); it != _events.end(); ++it)
	{
		if (it->first <= _fenceValue)
		{
			completedEvents.emplace_back(std::move(it->second));
		}
		else
		{
			if (it != _events.begin())
			{
				_events.erase(_events.begin(), --it);
			}
			eraseAll = false;
			break;
		}
	}

	if (eraseAll)
	{
		_events.clear();
	}

	lock.unlock();
	for (auto& src : completedEvents)
	{
		src.SetResult();
	}
}

uint64 NullRHIFence::GetCompletedValue() const
{
	return _fenceValue;
}

void NullRHIFence::Wait(uint64 fenceValue)
{
	std::unique_lock lock(_lock);
	if (_fenceValue >= fenceValue)
	{
		return;
	}

	auto& tcs = _events[fenceValue];
	if (!tcs.IsValid())
	{
		tcs = TaskCompletionSource<>::Create();
	}

	auto task = tcs.GetTask();
	lock.unlock();
	task.Wait();
}