// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12CommandBuffer.h"
#include "D3D12Graphics.h"
#include "CommandQueue.h"
#include "D3D12RaytracingRenderPipeline.h"
#include "D3D12GeometryRenderPipeline.h"

namespace Ayla
{
	D3D12CommandBuffer::D3D12CommandBuffer(D3D12Graphics* graphics)
		: m_Graphics(graphics)
	{
		HR(graphics->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandPool)));
#if DO_CHECK
		HR(m_CommandPool->SetName(L"D3D12CommandBuffer.m_CommandPool"));
#endif
		HR(graphics->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandPool.Get(), nullptr, IID_PPV_ARGS(&m_CommandBuffer)));
#if DO_CHECK
		HR(m_CommandBuffer->SetName(L"D3D12CommandBuffer.m_CommandBuffer"));
#endif
		HR(m_CommandBuffer->Close());
	}

	D3D12CommandBuffer::~D3D12CommandBuffer() noexcept
	{
	}

	void D3D12CommandBuffer::Dispose() noexcept
	{
		m_CommandPool.Reset();
		m_CommandBuffer.Reset();
	}

	void D3D12CommandBuffer::BeginCommands_Implementation()
	{
		HR(m_CommandPool->Reset());
		HR(m_CommandBuffer->Reset(m_CommandPool.Get(), nullptr));
	}

	void D3D12CommandBuffer::EndCommands_Implementation()
	{
		HR(m_CommandBuffer->Close());

		ID3D12CommandList* targetCommandBuffer = m_CommandBuffer.Get();
		auto& queue = m_Graphics->GetCommandQueue();
		queue.GetQueue()->ExecuteCommandLists(1, &targetCommandBuffer);
		m_FenceValue = queue.Signal();
	}

	void D3D12CommandBuffer::SetRenderPipeline(RenderPipeline* renderPipeline)
	{
		if (auto* pso1 = dynamic_cast<D3D12RaytracingRenderPipeline*>(renderPipeline))
		{
			m_CommandBuffer->SetPipelineState1(pso1->GetPipelineStateObject());
			return;
		}
		else if (auto* ps = dynamic_cast<D3D12GeometryRenderPipeline*>(renderPipeline))
		{
			m_CommandBuffer->SetGraphicsRootSignature(ps->GetRootSignature());
			m_CommandBuffer->SetPipelineState(ps->GetPipelineState());
			return;
		}
	}

	void D3D12CommandBuffer::WaitForCompletion(const TimeSpan& timeout)
	{
		auto& queue = m_Graphics->GetCommandQueue();
		queue.WaitForCompletion(m_FenceValue, timeout);
	}
}