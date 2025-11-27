// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12CommandBuffer.h"
#include "D3D12Graphics.h"

namespace Ayla
{
	D3D12CommandBuffer::D3D12CommandBuffer(D3D12Graphics* graphics)
		: m_Queue(graphics->GetCommandQueue())
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
		m_Queue->ExecuteCommandLists(1, &targetCommandBuffer);
	}
}