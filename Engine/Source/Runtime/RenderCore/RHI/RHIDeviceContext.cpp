// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Internal.h"

import std.core;
import SC.Runtime.RenderCore;
import SC.Runtime.Core;

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

void RHIDeviceContext::DrawIndexedInstanced(uint32 indexCountPerInstance, uint32 instanceCount, uint32 startIndexLocation, int32 baseVertexLocation, uint32 startInstanceLocation)
{
	_commandList->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void RHIDeviceContext::DrawInstanced(uint32 vertexCountPerInstance, uint32 instanceCount, int32 baseVertexLocation, uint32 startInstanceLocation)
{
	_commandList->DrawInstanced(vertexCountPerInstance, instanceCount, baseVertexLocation, startInstanceLocation);
}

void RHIDeviceContext::IASetPrimitiveTopology(ERHIPrimitiveTopology topology)
{
	_commandList->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)topology);
}

void RHIDeviceContext::SetGraphicsShader(RHIShader* shader)
{
	_commandList->SetGraphicsRootSignature(shader->GetRootSignature());
	_commandList->SetPipelineState(shader->GetPipelineState());
}

void RHIDeviceContext::SwapAllocator(ComPtr<ID3D12CommandAllocator>&& swap)
{
	ComPtr<ID3D12CommandAllocator> t = move(_allocator);
	_allocator = move(swap);
	swap = move(t);
}