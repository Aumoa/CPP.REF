// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHI/RHIDeviceContext.h"
#include "InternalComPtr.h"
#include "RHI/LogRHI.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHIShader.h"
#include "RHI/RHIRenderTargetView.h"
#include "RHI/RHIDepthStencilView.h"
#include "RHI/RHIResource.h"

using namespace std;

RHIDeviceContext::RHIDeviceContext(RHIDevice* device, ERHICommandType commandType) : Super(device)
	, _type(commandType)
{
	ID3D12Device* d3ddev = device->GetDevice();
	HR(LogRHI, d3ddev->CreateCommandAllocator((D3D12_COMMAND_LIST_TYPE)commandType, IID_PPV_ARGS(&_allocator)));
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

void RHIDeviceContext::OMSetRenderTargets(RHIRenderTargetView* rtv, int32 rtvStart, int32 count, RHIDepthStencilView* dsv, int32 dsvStart)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = rtv->GetCPUDescriptorHandle(rtvStart);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
	if (dsv != nullptr)
	{
		dsvHandle = dsv->GetCPUDescriptorHandle(dsvStart);
	}

	_commandList->OMSetRenderTargets(count, &handle, FALSE, dsv != nullptr ? &dsvHandle : nullptr);
}

void RHIDeviceContext::ClearRenderTargetView(RHIRenderTargetView* rtv, int32 index, const Color& color)
{
	_commandList->ClearRenderTargetView(rtv->GetCPUDescriptorHandle(index), (const FLOAT*)&color, 0, nullptr);
}

void RHIDeviceContext::ClearDepthStencilView(RHIDepthStencilView* rtv, int32 index, optional<float> depth, optional<uint8> stencil)
{
	D3D12_CLEAR_FLAGS flags = depth.has_value() ? D3D12_CLEAR_FLAG_DEPTH : (D3D12_CLEAR_FLAGS)0;
	flags |= stencil.has_value() ? D3D12_CLEAR_FLAG_STENCIL : (D3D12_CLEAR_FLAGS)0;
	_commandList->ClearDepthStencilView(rtv->GetCPUDescriptorHandle(index), flags, depth.value_or(0), stencil.value_or(0), 0, nullptr);
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

void RHIDeviceContext::SetGraphicsRootConstantBufferView(uint32 index, uint64 bufferLocation)
{
	_commandList->SetGraphicsRootConstantBufferView(index, bufferLocation);
}

void RHIDeviceContext::SetGraphicsRoot32BitConstants(uint32 index, uint32 num32BitsToSet, const void* srcData, uint32 destOffsetIn32BitValues)
{
	_commandList->SetGraphicsRoot32BitConstants(index, num32BitsToSet, srcData, destOffsetIn32BitValues);
}

ID3D12CommandList* RHIDeviceContext::GetCommandList() const
{
	return _commandList.Get();
}

void RHIDeviceContext::SwapAllocator(ComPtr<ID3D12CommandAllocator>&& swap)
{
	ComPtr<ID3D12CommandAllocator> t = move(_allocator);
	_allocator = move(swap);
	swap = move(t);
}