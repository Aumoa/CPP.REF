// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "CommandQueue.h"

namespace Ayla
{
	CommandQueue::CommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE commandType)
		: m_FenceValue(0)
		, m_FenceEvent(CreateEventExW(NULL, NULL, 0, GENERIC_ALL))
	{
		D3D12_COMMAND_QUEUE_DESC desc =
		{
			.Type = commandType,
			.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
			.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
			.NodeMask = 0
		};

		HR(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_Queue)));
		DXSetName(m_Queue);
		HR(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
		DXSetName(m_Fence);
	}

	CommandQueue::~CommandQueue() noexcept
	{
		CloseHandle(m_FenceEvent);
		m_FenceEvent = NULL;
	}

	uint64 CommandQueue::Signal()
	{
		auto fenceValue = ++m_FenceValue;
		HR(m_Queue->Signal(m_Fence.Get(), fenceValue));
		return fenceValue;
	}

	void CommandQueue::WaitForCompletion(std::optional<uint64> fenceValue, std::optional<TimeSpan> timeout)
	{
		if (fenceValue.has_value() == false)
		{
			fenceValue = m_FenceValue;
		}

		if (fenceValue.value() > m_Fence->GetCompletedValue())
		{
			HR(m_Fence->SetEventOnCompletion((UINT64)fenceValue.value(), m_FenceEvent));
			auto w = WaitForSingleObject(m_FenceEvent, timeout.has_value() ? (DWORD)timeout->GetTotalMilliseconds() : INFINITE);
			check(w == WAIT_OBJECT_0);
		}
	}
}