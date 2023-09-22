// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12CommandSet.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Global.h"
#include "D3D12RHI/D3D12Graphics.h"
#include "D3D12RHI/D3D12Viewport.h"
#include "D3D12RHI/D3D12Texture2D.h"
#include "D3D12RHI/D3D12Shader.h"
#include "Numerics/VectorInterface/Color.h"

ND3D12CommandSet::ND3D12CommandSet()
{
}

void ND3D12CommandSet::BeginFrame(const NRHIShader* pInitShader)
{
	const ND3D12Shader* pShader = static_cast<const ND3D12Shader*>(pInitShader);
	ID3D12PipelineState* pPipelineState = nullptr;
	if (pShader)
	{
		pPipelineState = pShader->GetPipelineState();
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
	auto& dVP = static_cast<const ND3D12Viewport&>(InViewport);
	int32 Index = dVP.GetCurrentBackBufferIndex();
	ID3D12Resource* pBufferResource = dVP.GetBackBuffer(Index).GetResource();

	D3D12_RESOURCE_BARRIER ResourceBarrier =
	{
		.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
		.Transition =
		{
			.pResource = pBufferResource,
			.Subresource = 0,
			.StateBefore = D3D12_RESOURCE_STATE_PRESENT,
			.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET
		}
	};

	CommandList->ResourceBarrier(1, &ResourceBarrier);

	D3D12_CPU_DESCRIPTOR_HANDLE hRTV = dVP.GetRTVHandle(Index);
	CommandList->OMSetRenderTargets(1, &hRTV, FALSE, nullptr);

	if (bClear)
	{
		CommandList->ClearRenderTargetView(hRTV, (FLOAT*)&NamedColors::Red, 0, nullptr);
	}
}

void ND3D12CommandSet::EndRender(const NRHIViewport& InViewport)
{
	auto& dVP = static_cast<const ND3D12Viewport&>(InViewport);
	int32 Index = dVP.GetCurrentBackBufferIndex();
	ID3D12Resource* pBufferResource = dVP.GetBackBuffer(Index).GetResource();

	D3D12_RESOURCE_BARRIER ResourceBarrier =
	{
		.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
		.Transition =
		{
			.pResource = pBufferResource,
			.Subresource = 0,
			.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET,
			.StateAfter = D3D12_RESOURCE_STATE_PRESENT
		}
	};

	CommandList->ResourceBarrier(1, &ResourceBarrier);
}

#endif