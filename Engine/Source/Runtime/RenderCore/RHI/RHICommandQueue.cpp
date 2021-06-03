// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Internal.h"

import SC.Runtime.RenderCore;

RHICommandQueue::RHICommandQueue(RHIDevice* device, ERHICommandType commandType) : Super(device)
{
	ID3D12Device* d3ddev = device->GetDevice();

	D3D12_COMMAND_QUEUE_DESC desc =
	{
		.Type = (D3D12_COMMAND_LIST_TYPE)commandType
	};

	HR(LogRHI, d3ddev->CreateCommandQueue(&desc, IID_PPV_ARGS(&_queue)));
}

RHICommandQueue::~RHICommandQueue()
{
}