// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "D3D12CommandList.h"
#include "RHI/IRHIDeferredCommandList.h"

#include "D3D12Minimal.h"

namespace SC::Runtime::Game::D3D12RHI
{
	class D3D12DeferredCommandList : public D3D12CommandList, virtual public RHI::IRHIDeferredCommandList
	{
	public:
		using Super = D3D12CommandList;
		using This = D3D12DeferredCommandList;

	private:
		ComPtr<ID3D12Device> device;
		ComPtr<ID3D12CommandAllocator> commandAllocator[2];
		ComPtr<ID3D12GraphicsCommandList> commandList;

		size_t currentAllocatorIndex;

	public:
		D3D12DeferredCommandList(ID3D12Device* device);
		~D3D12DeferredCommandList() override;

		virtual void BeginCommand();
		virtual void EndCommand() override;

		ID3D12GraphicsCommandList* CommandList_get() override;

	private:
		ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type);
		ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ID3D12Device* device, ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type);
		void MoveAllocatorNext();
	};
}