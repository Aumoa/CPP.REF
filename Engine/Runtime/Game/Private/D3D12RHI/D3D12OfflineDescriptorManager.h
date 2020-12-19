// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "D3D12Minimal.h"
#include <mutex>

namespace SC::Runtime::Game::D3D12RHI
{
	class D3D12DescriptorAllocator;
	struct D3D12OfflineDescriptorIndex;

	class D3D12OfflineDescriptorManager : public Core::Object
	{
	public:
		using Super = Core::Object;
		using This = D3D12OfflineDescriptorManager;

	private:
		const D3D12_DESCRIPTOR_HEAP_TYPE Type;
		const uint32 NumDescriptorsPerAllocator;

		ID3D12Device* device;
		std::vector<Core::TRefPtr<D3D12DescriptorAllocator>> heapAllocators;
		std::mutex locker;

	public:
		D3D12OfflineDescriptorManager(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32 numDescriptorsPerAllocator);
		~D3D12OfflineDescriptorManager() override;

		D3D12OfflineDescriptorIndex Alloc();
		void Free(const D3D12OfflineDescriptorIndex& index);

	private:
		Core::TRefPtr<D3D12DescriptorAllocator> NewAllocator();
	};
}