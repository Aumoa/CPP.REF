// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "D3D12Minimal.h"
#include <mutex>
#include <queue>
#include <vector>

namespace SC::Runtime::Game::D3D12RHI
{
	struct D3D12DescriptorIndex;

	class D3D12DescriptorAllocator : virtual public Core::Object
	{
	public:
		using Super = Core::Object;
		using This = D3D12DescriptorAllocator;

	private:
		ID3D12Device* const DeviceRef;
		const uint32 IncrementSize;

		std::mutex locker;
		ID3D12DescriptorHeap* descriptorHeap;
		std::queue<size_t> descriptor_pool;
		std::vector<size_t> rev_pool;

	public:
		D3D12DescriptorAllocator(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, size_t numDescriptors, bool bIsOffline);
		~D3D12DescriptorAllocator() override;

		D3D12DescriptorIndex Alloc();
		void Free(const D3D12DescriptorIndex& index);

		const D3D12_DESCRIPTOR_HEAP_TYPE Type;
		const size_t Count;

	private:
		void InitializePool();
	};
}