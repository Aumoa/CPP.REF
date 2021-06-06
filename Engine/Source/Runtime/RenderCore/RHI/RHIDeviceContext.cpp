// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Internal.h"

import std.core;
import SC.Runtime.RenderCore;

using namespace std;

RHIDeviceContext::RHIDeviceContext(RHIDevice* device, ERHICommandType commandType) : Super(device)
	, _type(commandType)
{
	ID3D12Device* d3ddev = device->GetDevice();
	HR(LogRHI, d3ddev->CreateCommandAllocator((D3D12_COMMAND_LIST_TYPE)commandType, IID_PPV_ARGS(&_allocator)));
}

RHIDeviceContext::~RHIDeviceContext()
{
}

void RHIDeviceContext::Begin()
{
	HR_E(LogRHI, _allocator->Reset());

	if (_commandList)
	{
		HR_E(LogRHI, _commandList->Reset(_allocator.Get(), nullptr));
	}
	else
	{
		ID3D12Device* d3ddev = GetDevice()->GetDevice();
		HR(LogRHI, d3ddev->CreateCommandList(0, (D3D12_COMMAND_LIST_TYPE)_type, _allocator.Get(), nullptr, IID_PPV_ARGS(&_commandList)));
	}
}

void RHIDeviceContext::End()
{
	HR_E(LogRHI, _commandList->Close());
}

void RHIDeviceContext::SwapAllocator(ComPtr<ID3D12CommandAllocator>&& swap)
{
	ComPtr<ID3D12CommandAllocator> t = move(_allocator);
	_allocator = move(swap);
	swap = move(t);
}