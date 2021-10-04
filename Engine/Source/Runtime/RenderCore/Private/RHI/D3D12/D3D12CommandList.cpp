// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12CommandList.h"
#include "D3D12Device.h"
#include "D3D12RenderTargetView.h"
#include "D3D12DepthStencilView.h"
#include "D3D12ShaderResourceView.h"
#include "D3D12Shader.h"
#include "D3D12DescriptorHeap.h"

SD3D12CommandList::SD3D12CommandList(SDXGIFactory* factory, SD3D12Device* device) : Super(factory, device)
{
}

void SD3D12CommandList::Begin(int32 maxSrvCount, int32 maxSamplerCount)
{
	ID3D12CommandAllocator* allocator = _device->GetThreadPrimaryAllocator();

	if (_commandList)
	{
		HR(_commandList->Reset(allocator, nullptr));
	}
	else
	{
		// For the first calling, we will create command list instance on demanding.
		auto dev = _device->Get<ID3D12Device>();
		HR(dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator, nullptr, IID_PPV_ARGS(&_commandList)));
	}

	std::vector<ID3D12DescriptorHeap*> heaps;
	heaps.reserve(2);

	if (maxSrvCount != 0)
	{
		_heapForSRV = _device->GetThreadPrimarySrvHeap(maxSrvCount);
		heaps.emplace_back(_heapForSRV->Get<ID3D12DescriptorHeap>());
	}

	if (maxSamplerCount != 0)
	{
		_heapForSampler = _device->GetThreadPrimarySamplerHeap(maxSamplerCount);
		heaps.emplace_back(_heapForSampler->Get<ID3D12DescriptorHeap>());
	}

	_commandList->SetDescriptorHeaps((UINT)heaps.size(), heaps.data());
}

void SD3D12CommandList::End()
{
	HR(_commandList->Close());
}

#define DEFINE_CALL_DIRECT_OneParam(Func, T1)	\
void SD3D12CommandList::Func(T1 N1)				\
{												\
	_commandList->Func(Conv(N1));				\
}

#define DEFINE_CALL_DIRECT_TwoParams(Func, T1, T2)	\
void SD3D12CommandList::Func(T1 N1, T2 N2)			\
{													\
	_commandList->Func(Conv(N1), Conv(N2));			\
}

#define DEFINE_CALL_DIRECT_FourParams(Func, T1, T2, T3, T4)			\
void SD3D12CommandList::Func(T1 N1, T2 N2, T3 N3, T4 N4)			\
{																	\
	_commandList->Func(Conv(N1), Conv(N2), Conv(N3), Conv(N4));		\
}

#define DEFINE_CALL_DIRECT_FiveParams(Func, T1, T2, T3, T4, T5)				\
void SD3D12CommandList::Func(T1 N1, T2 N2, T3 N3, T4 N4, T5 N5)				\
{																			\
	_commandList->Func(Conv(N1), Conv(N2), Conv(N3), Conv(N4), Conv(N5));	\
}

#define DEFINE_CALL_DIRECT_SixParams(Func, T1, T2, T3, T4, T5, T6)					\
void SD3D12CommandList::Func(T1 N1, T2 N2, T3 N3, T4 N4, T5 N5, T6 N6)				\
{																					\
	_commandList->Func(Conv(N1), Conv(N2), Conv(N3), Conv(N4), Conv(N5), Conv(N6));	\
}

#define GET_VIEW(View, Index) (View ? Cast<SD3D12View>(View)->GetHandle(Index) : D3D12_CPU_DESCRIPTOR_HANDLE())

DEFINE_CALL_DIRECT_FiveParams(DrawIndexedInstanced, uint32, uint32, uint32, int32, uint32);
DEFINE_CALL_DIRECT_FourParams(DrawInstanced, uint32, uint32, uint32, uint32);
DEFINE_CALL_DIRECT_OneParam(IASetPrimitiveTopology, ERHIPrimitiveTopology);

void SD3D12CommandList::OMSetRenderTargets(IRHIRenderTargetView* rtv, int32 indexOfRTV, int32 count, IRHIDepthStencilView* dsv, int32 indexOfDSV)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtv_h = GET_VIEW(rtv, indexOfRTV);
	D3D12_CPU_DESCRIPTOR_HANDLE dsv_h = GET_VIEW(dsv, indexOfDSV);

	_commandList->OMSetRenderTargets((UINT)count, &rtv_h, TRUE, dsv ? &dsv_h : nullptr);
}

void SD3D12CommandList::ClearRenderTargetView(IRHIRenderTargetView* rtv, int32 indexOf, const Color& color)
{
	_commandList->ClearRenderTargetView(GET_VIEW(rtv, indexOf), reinterpret_cast<const FLOAT*>(&color.R), 0, nullptr);
}

void SD3D12CommandList::ClearDepthStencilView(IRHIDepthStencilView* dsv, int32 indexOf, std::optional<float> depth, std::optional<uint8> stencil)
{
	D3D12_CLEAR_FLAGS flags = static_cast<D3D12_CLEAR_FLAGS>(0);
	if (depth.has_value())
	{
		flags |= D3D12_CLEAR_FLAG_DEPTH;
	}
	if (stencil.has_value())
	{
		flags |= D3D12_CLEAR_FLAG_STENCIL;
	}

	check(flags);
	_commandList->ClearDepthStencilView(GET_VIEW(dsv, indexOf), flags, depth.value_or(1.0f), stencil.value_or(0), 0, nullptr);
}

void SD3D12CommandList::RSSetScissorRects(std::span<const RHIScissorRect> scissorRects)
{
	_commandList->RSSetScissorRects((UINT)scissorRects.size(), reinterpret_cast<const D3D12_RECT*>(&scissorRects.front()));
}

void SD3D12CommandList::RSSetViewports(std::span<const RHIViewport> viewports)
{
	_commandList->RSSetViewports((UINT)viewports.size(), reinterpret_cast<const D3D12_VIEWPORT*>(&viewports.front()));
}

void SD3D12CommandList::ResourceBarrier(std::span<const RHIResourceBarrier> barriers)
{
	std::vector barriers_conv = Conv(barriers);
	_commandList->ResourceBarrier((UINT)barriers_conv.size(), barriers_conv.data());
}

void SD3D12CommandList::IASetVertexBuffers(uint32 slotIndex, std::span<const RHIVertexBufferView> views)
{
	std::vector views_conv = Conv(views);
	_commandList->IASetVertexBuffers(slotIndex, (UINT)views_conv.size(), views_conv.data());
}

DEFINE_CALL_DIRECT_OneParam(IASetIndexBuffer, const RHIIndexBufferView&);

void SD3D12CommandList::SetGraphicsShader(IRHIShader* shader)
{
	auto shader_s = Cast<SD3D12Shader>(shader);
	_commandList->SetGraphicsRootSignature(shader_s->Get<ID3D12RootSignature>());
	_commandList->SetPipelineState(shader_s->Get<ID3D12PipelineState>());
}

DEFINE_CALL_DIRECT_TwoParams(SetGraphicsRootConstantBufferView, uint32, uint64);
DEFINE_CALL_DIRECT_FourParams(SetGraphicsRoot32BitConstants, uint32, uint32, const void*, uint32);
DEFINE_CALL_DIRECT_TwoParams(SetGraphicsRootShaderResourceView, uint32, uint64);

void SD3D12CommandList::SetGraphicsRootShaderResourceView(uint32 index, IRHIShaderResourceView* view, int32 indexOf, int32 count)
{
	check(_heapForSRV->Slack() >= count);

	auto view_h = GET_VIEW(view, indexOf);
	D3D12_GPU_DESCRIPTOR_HANDLE hgpu = _heapForSRV->GetGPUHandle();
	D3D12_CPU_DESCRIPTOR_HANDLE hcpu = _heapForSRV->GetCPUHandle();
	_heapForSRV->Increment(count);

	auto dev = _device->Get<ID3D12Device>();
	dev->CopyDescriptorsSimple((UINT)count, hcpu, view_h, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	_commandList->SetGraphicsRootDescriptorTable(index, hgpu);
}

void SD3D12CommandList::ExecuteCommandLists(std::span<IRHIDeviceContext*> deviceContexts)
{
	SE_LOG(LogDirectX, Fatal, L"ExecuteCommandLists must be called with immediate context.");
}

void SD3D12CommandList::PendingGarbageObject(SObject* object)
{
	object->SetOuter(this);
	_pendingObjects.emplace_back(object);
}

void SD3D12CommandList::UpdateSubresource(IRHIResource* resource, uint32 subresource, const RHISubresourceData& data)
{
	auto resource_s = Cast<SD3D12Resource>(resource);
	ID3D12Resource* uploadBuf = resource_s->GetUploadBuf();
	if (uploadBuf == nullptr)
	{
		SE_LOG(LogDirectX, Fatal, L"Resource is not created with ERHIBufferUsage::Dynamic flag.");
		return;
	}

	void* ptr;
	HR(uploadBuf->Map(subresource, nullptr, &ptr));
}

DEFINE_CALL_DIRECT_SixParams(CopyTextureRegion, const D3D12_TEXTURE_COPY_LOCATION*, UINT, UINT, UINT, const D3D12_TEXTURE_COPY_LOCATION*, const D3D12_BOX*);
DEFINE_CALL_DIRECT_TwoParams(CopyResource, ID3D12Resource*, ID3D12Resource*);
DEFINE_CALL_DIRECT_TwoParams(ResourceBarrier, UINT, const D3D12_RESOURCE_BARRIER*);

std::vector<SObject*> SD3D12CommandList::ClearPendingObjects()
{
	return std::move(_pendingObjects);
}

#undef DEFINE_CALL_DIRECT_OneParam
#undef DEFINE_CALL_DIRECT_TwoParams
#undef DEFINE_CALL_DIRECT_FourParams
#undef DEFINE_CALL_DIRECT_FiveParams
#undef DEFINE_CALL_DIRECT_SixParams
#undef GET_VIEW