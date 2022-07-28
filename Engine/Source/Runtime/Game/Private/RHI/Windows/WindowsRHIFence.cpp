// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Windows/WindowsRHIFence.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHIDevice.h"

std::shared_ptr<RHIFence> WindowsRHIDevice::CreateFence()
{
	return std::shared_ptr<RHIFence>(new WindowsRHIFence(SharedFromThis()));
}

WindowsRHIFence::WindowsRHIFence(std::shared_ptr<WindowsRHIDevice> device)
	: RHIFence(device)
{
	ID3D12Device* dev = device->GetDevice();
	HR(dev->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));
}

WindowsRHIFence::~WindowsRHIFence() noexcept
{
	check(_eventsUsed == 0);
	for (auto& evt : _eventsPool)
	{
		CloseHandle(evt);
	}
}

uint64 WindowsRHIFence::GetCompletedValue() const
{
	return _fence->GetCompletedValue();
}

void WindowsRHIFence::Wait(uint64 fenceValue)
{
	if (GetCompletedValue() >= fenceValue)
	{
		return;
	}

	std::unique_lock lock(_lock);
	HANDLE hEvent = nullptr;
	if (_eventsPool.empty())
	{
		hEvent = CreateEventExW(nullptr, nullptr, 0, GENERIC_ALL);
	}
	else
	{
		hEvent = _eventsPool.back();
		_eventsPool.pop_back();
	}

	check(hEvent);

	if (GetCompletedValue() < fenceValue)
	{
		lock.unlock();

		_eventsUsed++;
		_fence->SetEventOnCompletion(fenceValue, hEvent);
		WaitForSingleObject(hEvent, INFINITE);
		_eventsUsed--;

		lock.lock();
	}

	_eventsPool.emplace_back(hEvent);
}

#endif