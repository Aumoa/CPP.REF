// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12CommandList.h"

#include "Engine.h"
#include "D3D12DeviceBundle.h"
#include "D3D12RenderTargetView.h"
#include "D3D12Resource.h"
#include "D3D12Shader.h"
#include "D3D12DepthStencilView.h"
#include "D3D12ShaderResourceView.h"
#include "D3D12RenderTarget.h"
#include "D3D12OnlineDescriptorPatch.h"
#include "D3D12UnorderedAccessView.h"
#include "Logging/LogMacros.h"
#include "RHI/RHIViewport.h"
#include "RHI/RHIMeshDrawCommand.h"
#include "RHI/RHIDispatchRaysDesc.h"
#include "RHI/IRHIShaderBindingTable.h"

using namespace std;

#define GET_RESOURCE(x) Cast<ID3D12ResourceBase>(x)->Resource
#define CHECK_PATCH \
if (currentPatch == nullptr)\
{\
	SE_LOG(LogD3D12RHI, Error, L"DescriptorPatch is not selected on current command list.");\
	return;\
}

D3D12CommandList::D3D12CommandList() : Super()
	, bHasBegunCommand(false)

	, currentPatch(nullptr)
{

}

D3D12CommandList::~D3D12CommandList()
{

}

void D3D12CommandList::BeginCommand()
{
	bHasBegunCommand = true;
	currentPatch = nullptr;
}

void D3D12CommandList::EndCommand()
{
	ConsumePendingDeferredCommands();
	HR(CommandList->Close());
	bHasBegunCommand = false;
}

void D3D12CommandList::SetRenderTargets(size_t count, IRHIRenderTargetView* rtv[], IRHIDepthStencilView* dsv)
{
	vector<D3D12_CPU_DESCRIPTOR_HANDLE> handles(count);
	for (size_t i = 0; i < count; ++i)
	{
		auto rtv_cast = Cast<D3D12RenderTargetView>(rtv[i]);
		handles[i] = rtv_cast->Handle;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
	if (dsv != nullptr)
	{
		dsvHandle = Cast<D3D12DepthStencilView>(dsv)->Handle;
	}

	CommandList->OMSetRenderTargets((UINT)count, handles.data(), FALSE, dsv == nullptr ? nullptr : &dsvHandle);
}

void D3D12CommandList::ClearRenderTargetView(IRHIRenderTargetView* rtv)
{
	ConsumePendingDeferredCommands();

	constexpr FLOAT ClearColor[4] = { 0, 0, 0, 0 };
	auto rtv_cast = Cast<D3D12RenderTargetView>(rtv);
	CommandList->ClearRenderTargetView(rtv_cast->Handle, ClearColor, 0, nullptr);
}

void D3D12CommandList::ResourceTransition(IRHIResource* resource, ERHIResourceStates beforeState, ERHIResourceStates afterState, size_t subresourceIndex)
{
	D3D12_RESOURCE_BARRIER barrier{ };
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = Cast<D3D12Resource>(resource)->Resource;
	barrier.Transition.StateBefore = (D3D12_RESOURCE_STATES)beforeState;
	barrier.Transition.StateAfter = (D3D12_RESOURCE_STATES)afterState;
	barrier.Transition.Subresource = (uint32)subresourceIndex;

	pendingBarriers.emplace_back(barrier);
}

void D3D12CommandList::CopyResource(IRHIResource* target, IRHIResource* source)
{
	ConsumePendingDeferredCommands();

	CommandList->CopyResource(GET_RESOURCE(target), GET_RESOURCE(source));
}

void D3D12CommandList::CopyBufferRegion(IRHIResource* inDstBuffer, uint64 inDstLocation, IRHIResource* inSrcBuffer, uint64 inSrcLocation, uint64 inNumBytes)
{
	ConsumePendingDeferredCommands();

	CommandList->CopyBufferRegion(GET_RESOURCE(inDstBuffer), inDstLocation, GET_RESOURCE(inSrcBuffer), inSrcLocation, inNumBytes);
}

void D3D12CommandList::SetGraphicsRoot32BitConstants(uint32 inParamIndex, const uint32* inBytes, size_t inNum32Bits, size_t location)
{
	if (inNum32Bits == 1)
	{
		CommandList->SetGraphicsRoot32BitConstant(inParamIndex, *inBytes, (UINT)location);
	}
	else
	{
		CommandList->SetGraphicsRoot32BitConstants(inParamIndex, (UINT)inNum32Bits, inBytes, (UINT)location);
	}
}

void D3D12CommandList::SetGraphicsRootShaderResource(uint32 inParamIndex, uint64 inVirtualAddress)
{
	CommandList->SetGraphicsRootShaderResourceView(inParamIndex, inVirtualAddress);
}

void D3D12CommandList::SetShader(IRHIShader* shader)
{
	auto d3d12Shader = Cast<D3D12Shader>(shader);
	d3d12Shader->InputShader(CommandList);
}

void D3D12CommandList::SetPrimitiveTopology(ERHIPrimitiveTopology primitiveTopology)
{
	CommandList->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)primitiveTopology);
}

void D3D12CommandList::SetScissorRects(const Rect& scissorRect)
{
	D3D12_RECT rc;
	rc.left = (INT)scissorRect.Min.X;
	rc.top = (INT)scissorRect.Min.Y;
	rc.right = (INT)scissorRect.Max.X;
	rc.bottom = (INT)scissorRect.Max.Y;

	CommandList->RSSetScissorRects(1, &rc);
}

void D3D12CommandList::SetViewports(const RHIViewport& scissorRect)
{
	D3D12_VIEWPORT vp;
	vp.TopLeftX = (FLOAT)scissorRect.X;
	vp.TopLeftY = (FLOAT)scissorRect.Y;
	vp.Width = (FLOAT)scissorRect.Width;
	vp.Height = (FLOAT)scissorRect.Height;
	vp.MinDepth = (FLOAT)scissorRect.Near;
	vp.MaxDepth = (FLOAT)scissorRect.Far;

	CommandList->RSSetViewports(1, &vp);
}

void D3D12CommandList::DrawMesh(const RHIMeshDrawCommand& command)
{
	ConsumePendingDeferredCommands();
	CommandList->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)command.Topology);

	if (command.VertexBufferVirtualAddress != 0)
	{
		D3D12_VERTEX_BUFFER_VIEW vbv = { };
		vbv.BufferLocation = command.VertexBufferVirtualAddress;
		vbv.StrideInBytes = command.VertexStride;
		vbv.SizeInBytes = vbv.StrideInBytes * command.VertexCount;
		CommandList->IASetVertexBuffers(0, 1, &vbv);
	}

	if (command.IndexBufferVirtualAddress != 0)
	{
		D3D12_INDEX_BUFFER_VIEW ibv = { };
		ibv.BufferLocation = command.IndexBufferVirtualAddress;
		ibv.SizeInBytes = (uint32)sizeof(uint32) * command.IndexCount;
		ibv.Format = DXGI_FORMAT_R32_UINT;
		CommandList->IASetIndexBuffer(&ibv);
		CommandList->DrawIndexedInstanced(command.IndexCount, 1, 0, 0, 0);
	}
	else
	{
		CommandList->DrawInstanced(command.VertexCount, 1, 0, 0);
	}
}

void D3D12CommandList::SetGraphicsRootConstantBufferView(uint32 inParamIndex, uint64 inVirtualAddress)
{
	CommandList->SetGraphicsRootConstantBufferView(inParamIndex, (D3D12_GPU_VIRTUAL_ADDRESS)inVirtualAddress);
}

void D3D12CommandList::ClearDepthStencilView(IRHIDepthStencilView* dsv, optional<float> depth, optional<uint8> stencil)
{
	ConsumePendingDeferredCommands();
	
	uint32 flags = depth.has_value() ? D3D12_CLEAR_FLAG_DEPTH : 0;
	flags |= stencil.has_value() ? D3D12_CLEAR_FLAG_STENCIL : 0;

	constexpr FLOAT ClearColor[4] = { 0, 0, 0, 0 };
	auto rtv_cast = Cast<D3D12DepthStencilView>(dsv);
	CommandList->ClearDepthStencilView(rtv_cast->Handle, (D3D12_CLEAR_FLAGS)flags, depth.value_or(1.0f), stencil.value_or(0), 0, nullptr);
}

void D3D12CommandList::SetGraphicsRootShaderResourceView(uint32 inRootParameterIndex, IRHIShaderResourceView* inSRV)
{
	CHECK_PATCH;
	size_t patchIndex = currentPatch->Patch(inSRV);
	D3D12_GPU_DESCRIPTOR_HANDLE handle = currentPatch->GetOnlineHandle(patchIndex);
	CommandList->SetGraphicsRootDescriptorTable(inRootParameterIndex, handle);
}

void D3D12CommandList::BeginRenderTarget(IRHIRenderTarget* renderTarget)
{
	ConsumePendingDeferredCommands();
	Cast<D3D12RenderTarget>(renderTarget)->BeginRender(CommandList);
}

void D3D12CommandList::EndRenderTarget(IRHIRenderTarget* renderTarget)
{
	ConsumePendingDeferredCommands();
	Cast<D3D12RenderTarget>(renderTarget)->EndRender(CommandList);
}

void D3D12CommandList::SetShaderDescriptorPatch(IRHIOnlineDescriptorPatch* inPatch)
{
	auto* patch = Cast<D3D12OnlineDescriptorPatch>(inPatch);
	ID3D12DescriptorHeap* heaps[] = { patch->Heap };
	CommandList->SetDescriptorHeaps(1, heaps);
	currentPatch = patch;
}

void D3D12CommandList::SetComputeRootUnorderedAccessView(uint32 inParamIndex, IRHIUnorderedAccessView* inUAV)
{
	CHECK_PATCH;
	size_t patchIndex = currentPatch->Patch(inUAV);
	D3D12_GPU_DESCRIPTOR_HANDLE handle = currentPatch->GetOnlineHandle(patchIndex);
	CommandList->SetComputeRootDescriptorTable(inParamIndex, handle);
}

void D3D12CommandList::DispatchRays(const RHIDispatchRaysDesc& dispatch)
{
	ConsumePendingDeferredCommands();

	D3D12_DISPATCH_RAYS_DESC dispatchDesc = { };
	dispatchDesc.RayGenerationShaderRecord.StartAddress = dispatch.SBT->RayGenerationStartAddress;
	dispatchDesc.RayGenerationShaderRecord.SizeInBytes = dispatch.SBT->RayGenerationBufferStride;
	dispatchDesc.Width = dispatch.Width;
	dispatchDesc.Height = dispatch.Height;
	dispatchDesc.Depth = 1;
	CommandList->DispatchRays(&dispatchDesc);
}

void D3D12CommandList::SetComputeRootShaderResourceView(uint32 inRootParameterIndex, IRHIShaderResourceView* inSRV)
{
	CHECK_PATCH;
	size_t patchIndex = currentPatch->Patch(inSRV);
	D3D12_GPU_DESCRIPTOR_HANDLE handle = currentPatch->GetOnlineHandle(patchIndex);
	CommandList->SetComputeRootDescriptorTable(inRootParameterIndex, handle);
}

void D3D12CommandList::SetComputeRootConstantBufferView(uint32 inParamIndex, uint64 inVirtualAddress)
{
	CommandList->SetComputeRootConstantBufferView(inParamIndex, (D3D12_GPU_VIRTUAL_ADDRESS)inVirtualAddress);
}

void D3D12CommandList::SetComputeRootShaderResource(uint32 inParamIndex, uint64 inVirtualAddress)
{
	CommandList->SetComputeRootShaderResourceView(inParamIndex, inVirtualAddress);
}

bool D3D12CommandList::HasBegunCommand_get() const
{
	return bHasBegunCommand;
}

void D3D12CommandList::ConsumePendingDeferredCommands()
{
	if (pendingBarriers.empty())
	{
		return;
	}

	{
		vector<D3D12_RESOURCE_BARRIER> barrier_swap;
		swap(barrier_swap, pendingBarriers);

		if (!barrier_swap.empty())
		{
			CommandList->ResourceBarrier((UINT)barrier_swap.size(), barrier_swap.data());
		}
	}
}