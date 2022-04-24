// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXCommandList.h"
#include "DirectXResource.h"
#include "DirectXRenderTargetView.h"
#include "DirectXCommandAllocator.h"
#include "DirectXPipelineState.h"

using namespace ::libty;

SDirectXCommandList::SDirectXCommandList(SDirectXDevice* Owner, ComPtr<ID3D12CommandAllocator> pAllocator, ComPtr<ID3D12GraphicsCommandList4> pCommandList)
	: Super(Owner)
	, pCommandList(std::move(pCommandList))
{
}

void SDirectXCommandList::Reset(IRHICommandAllocator* pAllocator, IRHIPipelineState* pInitialState)
{
	auto* sAllocator = Cast<SDirectXCommandAllocator>(pAllocator);
	auto* sInitialState = Cast<SDirectXPipelineState>(pInitialState);
	HR(pCommandList->Reset(sAllocator->pAllocator.Get(), sInitialState ? sInitialState->pPipelineState.Get() : nullptr));
}

void SDirectXCommandList::Close()
{
	HR(pCommandList->Close());
}

void SDirectXCommandList::ResourceBarrier(std::span<const RHIResourceBarrier> barriers)
{
	std::vector<D3D12_RESOURCE_BARRIER> r_barriers;
	ReplaceNativePointer(r_barriers, barriers);
	pCommandList->ResourceBarrier((UINT)r_barriers.size(), r_barriers.data());
}

void SDirectXCommandList::ClearRenderTargetView(IRHIRenderTargetView* pRTV, size_t indexOf, const Color& color, std::span<const RectN> rects)
{
	auto sRTV = Cast<SDirectXRenderTargetView>(pRTV);
	D3D12_CPU_DESCRIPTOR_HANDLE handle = sRTV->GetCPUHandle(indexOf);
	pCommandList->ClearRenderTargetView(handle, reinterpret_cast<const FLOAT*>(&color), (UINT)rects.size(), rects.size() == 0 ? nullptr : reinterpret_cast<const D3D12_RECT*>(rects.data()));
}

void SDirectXCommandList::Dispose(bool bDisposing)
{
	pCommandList.Reset();
	Super::Dispose(bDisposing);
}