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

void RHIDeviceContext::OMSetRenderTargets(RHIRenderTargetView* rtv)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = rtv->GetCPUDescriptorHandle();
	_commandList->OMSetRenderTargets(rtv->GetDescriptorCount(), &handle, 0, nullptr);
}

void RHIDeviceContext::OMSetRenderTargets(RHIRenderTargetView* rtv, int32 index, int32 count)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = rtv->GetCPUDescriptorHandle(index);
	_commandList->OMSetRenderTargets(count, &handle, 0, nullptr);
}

void RHIDeviceContext::ClearRenderTargetView(RHIRenderTargetView* rtv, const Color& color)
{
	_commandList->ClearRenderTargetView(rtv->GetCPUDescriptorHandle(), (const FLOAT*)&color, 0, nullptr);
}

void RHIDeviceContext::ClearRenderTargetView(RHIRenderTargetView* rtv, int32 index, const Color& color)
{
	_commandList->ClearRenderTargetView(rtv->GetCPUDescriptorHandle(index), (const FLOAT*)&color, 0, nullptr);
}

void RHIDeviceContext::RSSetScissorRects(int32 count, const RHIScissorRect* rects)
{
	_commandList->RSSetScissorRects((UINT)count, (const D3D12_RECT*)rects);
}

void RHIDeviceContext::RSSetViewports(int32 count, const RHIViewport* viewports)
{
	_commandList->RSSetViewports((UINT)count, (const D3D12_VIEWPORT*)viewports);
}

void RHIDeviceContext::TransitionBarrier(int32 count, const RHITransitionBarrier* barriers)
{
	vector<D3D12_RESOURCE_BARRIER> d3dbars(count);
	for (int32 i = 0; i < count; ++i)
	{
		auto& barrier = barriers[i];

		d3dbars[i] =
		{
			.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
			.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
			.Transition =
			{
				.pResource = barrier.Resource->GetResource(),
				.Subresource = barrier.SubresourceIndex,
				.StateBefore = (D3D12_RESOURCE_STATES)barrier.StateBefore,
				.StateAfter = (D3D12_RESOURCE_STATES)barrier.StateAfter
			}
		};
	}

	_commandList->ResourceBarrier(count, d3dbars.data());
}

void RHIDeviceContext::IASetVertexBuffers(uint32 startSlot, uint32 numViews, const RHIVertexBufferView* views)
{
	_commandList->IASetVertexBuffers(startSlot, numViews, (const D3D12_VERTEX_BUFFER_VIEW*)views);
}

void RHIDeviceContext::IASetIndexBuffer(const RHIIndexBufferView& view)
{
	_commandList->IASetIndexBuffer((const D3D12_INDEX_BUFFER_VIEW*)&view);
}

void RHIDeviceContext::SwapAllocator(ComPtr<ID3D12CommandAllocator>&& swap)
{
	ComPtr<ID3D12CommandAllocator> t = move(_allocator);
	_allocator = move(swap);
	swap = move(t);
}