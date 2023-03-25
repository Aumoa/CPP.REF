// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12CommandQueue.h"
#include "D3D12RHI/D3D12Graphics.h"

ND3D12CommandQueue::ND3D12CommandQueue(ND3D12Graphics* InGraphics)
	: Graphics(InGraphics)
{
	ID3D12Device* Dev = Graphics->GetDevice();

	D3D12_COMMAND_QUEUE_DESC QueueDesc =
	{
		.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
		.Priority = (INT)D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
		.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
		.NodeMask = 0
	};
	HR(Dev->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&Queue)));
}

ND3D12CommandQueue::~ND3D12CommandQueue() noexcept
{
}

NRHIGraphics* ND3D12CommandQueue::GetDevice() const
{
	return Graphics;
}

#endif