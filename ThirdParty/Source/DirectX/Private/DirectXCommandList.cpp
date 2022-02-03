// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXCommandList.h"
#include "DirectXResource.h"
#include "DirectXRenderTargetView.h"
#include "DirectXRenderPass.h"

GENERATE_BODY(SDirectXCommandList);

SDirectXCommandList::SDirectXCommandList(SDirectXDevice* Owner, ComPtr<ID3D12CommandAllocator> pAllocator, ComPtr<ID3D12GraphicsCommandList4> pCommandList)
	: Super(Owner)
	, pAllocator(std::move(pAllocator))
	, pCommandList(std::move(pCommandList))
{
}

void SDirectXCommandList::BeginRecord()
{
	pCommandList->Reset(pAllocator.Get(), nullptr);
}

void SDirectXCommandList::EndRecord()
{
	HR(pCommandList->Close());
}

void SDirectXCommandList::ResourceBarrier(std::span<const RHIResourceBarrier> InBarriers)
{
	std::vector<D3D12_RESOURCE_BARRIER> Barriers;
	Barriers.reserve(InBarriers.size());

	for (auto& Barrier : InBarriers)
	{
		switch (Barrier.index())
		{
		case 0:
		{
			auto& Transit = std::get<RHIResourceTransitionBarrier>(Barrier);
			Barriers.emplace_back() =
			{
				.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
				.Transition =
				{
					.pResource = Cast<SDirectXResource>(Transit.pResource)->pResource.Get(),
					.Subresource = 0,
					.StateBefore = (D3D12_RESOURCE_STATES)Transit.StateBefore,
					.StateAfter = (D3D12_RESOURCE_STATES)Transit.StateAfter,
				}
			};
			break;
		}
		case 1:
		{
			auto& Alias = std::get<RHIResourceAliasingBarrier>(Barrier);
			Barriers.emplace_back() =
			{
				.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING,
				.Aliasing =
				{
					.pResourceBefore = Cast<SDirectXResource>(Alias.pResourceBefore)->pResource.Get(),
					.pResourceAfter = Cast<SDirectXResource>(Alias.pResourceAfter)->pResource.Get()
				}
			};
			break;
		}
		case 2:
		{
			auto& UAV = std::get<RHIResourceUAVBarrier>(Barrier);
			Barriers.emplace_back() =
			{
				.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV,
				.UAV =
				{
					.pResource = Cast<SDirectXResource>(UAV.pResource)->pResource.Get()
				}
			};
			break;
		}
		}
	}

	pCommandList->ResourceBarrier((UINT)Barriers.size(), Barriers.data());
}

void SDirectXCommandList::ClearRenderTargetView(IRHIRenderTargetView* RTV, int32 IndexOf, const Color& InColor)
{
	auto* sRTV = Cast<SDirectXRenderTargetView>(RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE Handle = sRTV->GetCPUHandle(IndexOf);
	pCommandList->ClearRenderTargetView(Handle, (const FLOAT*)&InColor, 0, nullptr);
}

void SDirectXCommandList::BeginRenderPass(IRHIRenderPass* RenderPass, const RectN& ScissorRect, std::span<Color const> InColor)
{
	auto* sRenderPass = Cast<SDirectXRenderPass>(RenderPass);
	std::vector RenderTargets = sRenderPass->RenderTargets;

	size_t ClearIndex = 0;
	for (size_t i = 0; i < RenderTargets.size();)
	{
		auto& RenderTarget = RenderTargets[i++];
		if (RenderTarget.BeginningAccess.Type == D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR)
		{
			memcpy(RenderTarget.BeginningAccess.Clear.ClearValue.Color, &InColor[ClearIndex++], sizeof(Color));
		}
	}

	pCommandList->BeginRenderPass((UINT)RenderTargets.size(), RenderTargets.data(), nullptr, D3D12_RENDER_PASS_FLAG_NONE);
	RunningRenderPass = sRenderPass;
}

void SDirectXCommandList::EndRenderPass()
{
	pCommandList->EndRenderPass();
	pCommandList->ResourceBarrier((UINT)RunningRenderPass->TransitionBarriers.size(), RunningRenderPass->TransitionBarriers.data());
	RunningRenderPass = nullptr;
}

void SDirectXCommandList::Dispose(bool bDisposing)
{
	pAllocator.Reset();
	pCommandList.Reset();
	Super::Dispose(bDisposing);
}