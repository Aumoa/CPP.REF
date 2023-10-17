// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12CommandSet.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Global.h"
#include "D3D12RHI/D3D12Graphics.h"
#include "D3D12RHI/D3D12SwapChain.h"
#include "D3D12RHI/D3D12Texture2D.h"
#include "D3D12RHI/D3D12GraphicsPipelineState.h"
#include "D3D12RHI/D3D12RootSignature.h"
#include "D3D12RHI/D3D12DescriptorHeap.h"
#include "D3D12RHI/D3D12Viewport.h"
#include "Numerics/VectorInterface/Color.h"

ND3D12CommandSet::ND3D12CommandSet()
{
}

void ND3D12CommandSet::BeginFrame(const NRHIGraphicsPipelineState* pInitialPipeline)
{
	auto* pD3DPS = static_cast<const ND3D12GraphicsPipelineState*>(pInitialPipeline);
	ID3D12PipelineState* pPipelineState = nullptr;
	if (pD3DPS)
	{
		pPipelineState = pD3DPS->Get();
	}

	if (!Allocator && !CommandList)
	{
		ID3D12Device1* pDevice = ND3D12Global::GetDynamicRHI().GetDevice();
		HR(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator)));
		HR(pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator.Get(), pPipelineState, IID_PPV_ARGS(&CommandList)));
	}
	else
	{
		HR(Allocator->Reset());
		HR(CommandList->Reset(Allocator.Get(), pPipelineState));
	}
}

void ND3D12CommandSet::EndFrame()
{
	HR(CommandList->Close());
}

void ND3D12CommandSet::BeginRender(const NRHIViewport& InViewport, bool bClear)
{
	D3D12_RESOURCE_STATES DesiredState;
	D3D12_CPU_DESCRIPTOR_HANDLE hRTV;
	ID3D12Resource* pResource;

	if (auto* pSwapChain = dynamic_cast<const ND3D12SwapChain*>(&InViewport); pSwapChain)
	{
		DesiredState = D3D12_RESOURCE_STATE_PRESENT;
		int32 Index = pSwapChain->GetCurrentBackBufferIndex();
		pResource = pSwapChain->GetBackBuffer(Index).GetResource();
		hRTV = pSwapChain->GetRTVHandle(Index);
	}
	else
	{
		auto& dVP = static_cast<const ND3D12Viewport&>(InViewport);
		DesiredState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		pResource = dVP.GetResource().Get();
		hRTV = dVP.GetRTVHandle();
	}

	D3D12_RESOURCE_BARRIER ResourceBarrier =
	{
		.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
		.Transition =
		{
			.pResource = pResource,
			.Subresource = 0,
			.StateBefore = DesiredState,
			.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET
		}
	};

	CommandList->ResourceBarrier(1, &ResourceBarrier);
	CommandList->OMSetRenderTargets(1, &hRTV, FALSE, nullptr);

	if (bClear)
	{
		CommandList->ClearRenderTargetView(hRTV, (FLOAT*)&NamedColors::Transparent, 0, nullptr);
	}

	Vector2N VpSize = InViewport.GetViewportSize();
	D3D12_VIEWPORT Vp =
	{
		.TopLeftX = 0,
		.TopLeftY = 0,
		.Width = (FLOAT)VpSize.X,
		.Height = (FLOAT)VpSize.Y,
		.MinDepth = 0,
		.MaxDepth = 1.0f
	};
	D3D12_RECT ScRect =
	{
		.left = 0,
		.top = 0,
		.right = (LONG)VpSize.X,
		.bottom = (LONG)VpSize.Y
	};
	CommandList->RSSetViewports(1, &Vp);
	CommandList->RSSetScissorRects(1, &ScRect);
}

void ND3D12CommandSet::EndRender(const NRHIViewport& InViewport)
{
	D3D12_RESOURCE_STATES DesiredState;
	ID3D12Resource* pResource;

	if (auto* pSwapChain = dynamic_cast<const ND3D12SwapChain*>(&InViewport); pSwapChain)
	{
		DesiredState = D3D12_RESOURCE_STATE_PRESENT;
		int32 Index = pSwapChain->GetCurrentBackBufferIndex();
		pResource = pSwapChain->GetBackBuffer(Index).GetResource();
	}
	else
	{
		DesiredState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		pResource = static_cast<const ND3D12Viewport&>(InViewport).GetResource().Get();
	}

	D3D12_RESOURCE_BARRIER ResourceBarrier =
	{
		.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
		.Transition =
		{
			.pResource = pResource,
			.Subresource = 0,
			.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET,
			.StateAfter = DesiredState
		}
	};

	CommandList->ResourceBarrier(1, &ResourceBarrier);
}

void ND3D12CommandSet::SetDescriptorHeaps(std::span<NRHIDescriptorHeap* const> Heaps)
{
	static thread_local std::vector<ID3D12DescriptorHeap*> ppHeaps;
	ppHeaps.clear();

	ppHeaps.reserve(Heaps.size());
	for (auto& Heap : Heaps)
	{
		auto* pHeap = static_cast<ND3D12DescriptorHeap*>(Heap)->Get();
		if (pHeap)
		{
			ppHeaps.emplace_back(pHeap);
		}
	}

	CommandList->SetDescriptorHeaps((UINT)ppHeaps.size(), ppHeaps.data());
}

void ND3D12CommandSet::SetGraphicsRootSignature(NRHIRootSignature& InRS)
{
	ID3D12RootSignature* pRS = static_cast<ND3D12RootSignature&>(InRS).Get();
	CommandList->SetGraphicsRootSignature(pRS);
}

void ND3D12CommandSet::SetGraphicsRoot32BitConstants(int32 RootParameterIndex, int32 Num32BitValuesToSet, const void* pSrcData, int32 DestOffsetIn32BitValues)
{
	CommandList->SetGraphicsRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
}

void ND3D12CommandSet::SetGraphicsRootConstantBufferView(int32 RootParameterIndex, int64 BufferLocation)
{
	CommandList->SetGraphicsRootConstantBufferView(RootParameterIndex, BufferLocation);
}

void ND3D12CommandSet::SetGraphicsRootDescriptorTable(int32 RootParameterIndex, int64 VirtualHandleLocation)
{
	D3D12_GPU_DESCRIPTOR_HANDLE hView{ .ptr = (UINT64)VirtualHandleLocation };
	CommandList->SetGraphicsRootDescriptorTable(RootParameterIndex, hView);
}

void ND3D12CommandSet::DrawInstanced(bool bStrip, int32 InVertexCount, int32 InInstanceCount, int32 InVertexStart, int32 InInstanceStart)
{
	CommandList->IASetPrimitiveTopology(bStrip ? D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP : D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CommandList->DrawInstanced(InVertexCount, InInstanceCount, InVertexStart, InInstanceStart);
}

#endif