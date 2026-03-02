// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12Common.h"

namespace Ayla
{
	class CommandQueue
	{
	private:
		ComPtr<ID3D12CommandQueue> m_Queue;
		ComPtr<ID3D12Fence> m_Fence;
		uint64 m_FenceValue;
		HANDLE m_FenceEvent;

	public:
		CommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE commandType);
		~CommandQueue() noexcept;

		inline ID3D12CommandQueue* GetQueue() const noexcept { return m_Queue.Get(); }
		inline ID3D12Fence* GetFence() const noexcept { return m_Fence.Get(); }

		uint64 Signal();
		void WaitForCompletion(std::optional<uint64> fenceValue = std::nullopt, std::optional<TimeSpan> timeout = std::nullopt);
	};
}