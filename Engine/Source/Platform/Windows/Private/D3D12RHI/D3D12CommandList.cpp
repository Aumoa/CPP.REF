// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12CommandList.h"
#include "D3D12Device.h"
#include "D3D12RenderTargetView.h"
#include "D3D12DepthStencilView.h"
#include "D3D12ShaderResourceView.h"
#include "D3D12Shader.h"
#include "D3D12DescriptorHeap.h"

SD3D12CommandList::SD3D12CommandList(SDXGIFactory* factory, SD3D12Device* device) : Super(factory, device, nullptr)
{
}

void SD3D12CommandList::Begin()
{
	ID3D12CommandAllocator* allocator = _device->GetThreadPrimaryAllocator();

	if (CommandList)
	{
		HR(CommandList->Reset(allocator, nullptr));
	}
	else
	{
		// For the first calling, we will create command list instance on demanding.
		auto dev = _device->Get<ID3D12Device>();
		HR(dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator, nullptr, IID_PPV_ARGS(&CommandList)));
	}
}

void SD3D12CommandList::End()
{
	HR(CommandList->Close());
}

void SD3D12CommandList::SetDescriptorHeaps(int32 MaxSRVCount, int32 MaxSamplerCount)
{
	ID3D12DescriptorHeap* Heaps[2] = {};
	int32 NumHeaps = 0;

	if (MaxSRVCount)
	{
		HeapForSRV = _device->GetThreadPrimarySrvHeap(MaxSRVCount);
		Heaps[NumHeaps++] = HeapForSRV->Get<ID3D12DescriptorHeap>();
	}

	if (MaxSamplerCount)
	{
		HeapForSampler = _device->GetThreadPrimarySamplerHeap(MaxSamplerCount);
		Heaps[NumHeaps++] = HeapForSampler->Get<ID3D12DescriptorHeap>();
	}

	if (NumHeaps)
	{
		CommandList->SetDescriptorHeaps((UINT)NumHeaps, Heaps);
	}
}

#define DEFINE_CALL_DIRECT_OneParam(Func, T1)	\
void SD3D12CommandList::Func(T1 N1)				\
{												\
	CommandList->Func(Conv(N1));				\
}

#define DEFINE_CALL_DIRECT_TwoParams(Func, T1, T2)	\
void SD3D12CommandList::Func(T1 N1, T2 N2)			\
{													\
	CommandList->Func(Conv(N1), Conv(N2));			\
}

#define DEFINE_CALL_DIRECT_FourParams(Func, T1, T2, T3, T4)			\
void SD3D12CommandList::Func(T1 N1, T2 N2, T3 N3, T4 N4)			\
{																	\
	CommandList->Func(Conv(N1), Conv(N2), Conv(N3), Conv(N4));		\
}

#define DEFINE_CALL_DIRECT_FiveParams(Func, T1, T2, T3, T4, T5)				\
void SD3D12CommandList::Func(T1 N1, T2 N2, T3 N3, T4 N4, T5 N5)				\
{																			\
	CommandList->Func(Conv(N1), Conv(N2), Conv(N3), Conv(N4), Conv(N5));	\
}

#define DEFINE_CALL_DIRECT_SixParams(Func, T1, T2, T3, T4, T5, T6)					\
void SD3D12CommandList::Func(T1 N1, T2 N2, T3 N3, T4 N4, T5 N5, T6 N6)				\
{																					\
	CommandList->Func(Conv(N1), Conv(N2), Conv(N3), Conv(N4), Conv(N5), Conv(N6));	\
}

#define GET_VIEW(View, Index) (View ? Cast<SD3D12View>(View)->GetHandle(Index) : D3D12_CPU_DESCRIPTOR_HANDLE())

DEFINE_CALL_DIRECT_FiveParams(DrawIndexedInstanced, uint32, uint32, uint32, int32, uint32);
DEFINE_CALL_DIRECT_FourParams(DrawInstanced, uint32, uint32, uint32, uint32);
DEFINE_CALL_DIRECT_OneParam(IASetPrimitiveTopology, ERHIPrimitiveTopology);

void SD3D12CommandList::OMSetRenderTargets(IRHIRenderTargetView* rtv, int32 indexOfRTV, int32 count, IRHIDepthStencilView* dsv, int32 indexOfDSV)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtv_h = GET_VIEW(rtv, indexOfRTV);
	D3D12_CPU_DESCRIPTOR_HANDLE dsv_h = GET_VIEW(dsv, indexOfDSV);

	CommandList->OMSetRenderTargets((UINT)count, &rtv_h, TRUE, dsv ? &dsv_h : nullptr);
}

void SD3D12CommandList::ClearRenderTargetView(IRHIRenderTargetView* rtv, int32 indexOf, const Color& color)
{
	CommandList->ClearRenderTargetView(GET_VIEW(rtv, indexOf), reinterpret_cast<const FLOAT*>(&color.R), 0, nullptr);
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
	CommandList->ClearDepthStencilView(GET_VIEW(dsv, indexOf), flags, depth.value_or(1.0f), stencil.value_or(0), 0, nullptr);
}

void SD3D12CommandList::RSSetScissorRects(std::span<const RHIScissorRect> scissorRects)
{
	CommandList->RSSetScissorRects((UINT)scissorRects.size(), reinterpret_cast<const D3D12_RECT*>(&scissorRects.front()));
}

void SD3D12CommandList::RSSetViewports(std::span<const RHIViewport> viewports)
{
	CommandList->RSSetViewports((UINT)viewports.size(), reinterpret_cast<const D3D12_VIEWPORT*>(&viewports.front()));
}

void SD3D12CommandList::ResourceBarrier(std::span<const RHIResourceBarrier> barriers)
{
	std::vector barriers_conv = Conv(barriers);
	CommandList->ResourceBarrier((UINT)barriers_conv.size(), barriers_conv.data());
}

void SD3D12CommandList::IASetVertexBuffers(uint32 slotIndex, std::span<const RHIVertexBufferView> views)
{
	std::vector views_conv = Conv(views);
	CommandList->IASetVertexBuffers(slotIndex, (UINT)views_conv.size(), views_conv.data());
}

DEFINE_CALL_DIRECT_OneParam(IASetIndexBuffer, const RHIIndexBufferView&);

void SD3D12CommandList::SetGraphicsShader(IRHIShader* shader)
{
	auto shader_s = Cast<SD3D12Shader>(shader);
	CommandList->SetGraphicsRootSignature(shader_s->Get<ID3D12RootSignature>());
	CommandList->SetPipelineState(shader_s->Get<ID3D12PipelineState>());
}

DEFINE_CALL_DIRECT_TwoParams(SetGraphicsRootConstantBufferView, uint32, uint64);
DEFINE_CALL_DIRECT_FourParams(SetGraphicsRoot32BitConstants, uint32, uint32, const void*, uint32);
DEFINE_CALL_DIRECT_TwoParams(SetGraphicsRootShaderResourceView, uint32, uint64);

void SD3D12CommandList::SetGraphicsRootShaderResourceView(uint32 index, IRHIShaderResourceView* view, int32 indexOf, int32 count)
{
	check(HeapForSRV->Slack() >= count);

	auto view_h = GET_VIEW(view, indexOf);
	D3D12_GPU_DESCRIPTOR_HANDLE hgpu = HeapForSRV->GetGPUHandle();
	D3D12_CPU_DESCRIPTOR_HANDLE hcpu = HeapForSRV->GetCPUHandle();
	HeapForSRV->Increment(count);

	auto dev = _device->Get<ID3D12Device>();
	dev->CopyDescriptorsSimple((UINT)count, hcpu, view_h, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CommandList->SetGraphicsRootDescriptorTable(index, hgpu);
}

void SD3D12CommandList::PendingGarbageObject(SObject* object)
{
	object->SetOuter(this);
	PendingObjects.emplace_back(object);
}

void SD3D12CommandList::UpdateSubresource(IRHIResource* resource, uint32 subresource, const RHISubresourceData& data)
{
	auto resource_s = Cast<SD3D12Resource>(resource);
	ID3D12Resource* uploadBuf = resource_s->GetUploadBuf();
	if (uploadBuf == nullptr)
	{
		SE_LOG(LogWindows, Fatal, L"Resource is not created with ERHIBufferUsage::Dynamic flag.");
		return;
	}

	void* ptr;
	HR(uploadBuf->Map(subresource, nullptr, &ptr));
}

void SD3D12CommandList::CopyResource(IRHIResource* DstResource, IRHIResource* SrcResource)
{
	auto DstResource_s = Cast<SD3D12Resource>(DstResource);
	auto SrcResource_s = Cast<SD3D12Resource>(SrcResource);

	CommandList->CopyResource(DstResource_s->Get<ID3D12Resource>(), SrcResource_s->Get<ID3D12Resource>());
}

uint64 SD3D12CommandList::ExecuteCommandLists(std::span<IRHIDeviceContext*> deviceContexts, bool bSignal)
{
	SE_LOG(LogWindows, Fatal, L"ExecuteCommandLists must be called with immediate context.");
	return 0;
}

DEFINE_CALL_DIRECT_SixParams(CopyTextureRegion, const D3D12_TEXTURE_COPY_LOCATION*, UINT, UINT, UINT, const D3D12_TEXTURE_COPY_LOCATION*, const D3D12_BOX*);
DEFINE_CALL_DIRECT_TwoParams(CopyResource, ID3D12Resource*, ID3D12Resource*);
DEFINE_CALL_DIRECT_TwoParams(ResourceBarrier, UINT, const D3D12_RESOURCE_BARRIER*);

std::vector<SObject*> SD3D12CommandList::ClearPendingObjects()
{
	return std::move(PendingObjects);
}

#undef DEFINE_CALL_DIRECT_OneParam
#undef DEFINE_CALL_DIRECT_TwoParams
#undef DEFINE_CALL_DIRECT_FourParams
#undef DEFINE_CALL_DIRECT_FiveParams
#undef DEFINE_CALL_DIRECT_SixParams
#undef GET_VIEW