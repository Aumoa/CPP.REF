// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Windows/WindowsRHICommandQueue.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHIDevice.h"
#include "RHI/Windows/WindowsRHIFence.h"
#include "RHI/Windows/WindowsRHICommandList.h"

std::shared_ptr<RHICommandQueue> WindowsRHIDevice::CreateCommandQueue()
{
	return std::shared_ptr<RHICommandQueue>(new WindowsRHICommandQueue(SharedFromThis()));
}

WindowsRHICommandQueue::WindowsRHICommandQueue(std::shared_ptr<WindowsRHIDevice> device)
	: RHICommandQueue(device)
{
	ID3D12Device* d3ddev = WinGetr(device);
	D3D12_COMMAND_QUEUE_DESC queueDesc =
	{
		.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
		.Priority = 0,
		.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
		.NodeMask = 0
	};
	HR(d3ddev->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_queue)));
}

WindowsRHICommandQueue::~WindowsRHICommandQueue() noexcept
{
}

void WindowsRHICommandQueue::Signal(std::shared_ptr<RHIFence> fence, uint64 fenceValue)
{
	auto wFence = std::static_pointer_cast<WindowsRHIFence>(fence);
	auto pFence = wFence->GetFence();

	HR(_queue->Signal(pFence, fenceValue));
}

void WindowsRHICommandQueue::ExecuteCommandLists(std::span<std::shared_ptr<RHICommandList>> cmdLists)
{
	static thread_local std::vector<ID3D12CommandList*> raws;
	raws.reserve(cmdLists.size());

	for (auto& ptr : cmdLists)
	{
		raws.emplace_back(CastChecked<WindowsRHICommandList>(ptr)->GetCommandList().Get());
	}

	_queue->ExecuteCommandLists((UINT)raws.size(), raws.data());
}

#endif