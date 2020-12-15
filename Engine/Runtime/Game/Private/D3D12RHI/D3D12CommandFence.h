// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHICommandFence.h"

#include "D3D12Minimal.h"

namespace SC::Runtime::Game::D3D12RHI
{
	class D3D12CommandFence : virtual public Core::Object, virtual public RHI::IRHICommandFence
	{
	public:
		using Super = Core::Object;

	private:
		ComPtr<ID3D12Fence> fence;
		uint64 fenceValue;
		HANDLE hEvent;

	public:
		D3D12CommandFence(ID3D12Device* device);
		~D3D12CommandFence() override;

		virtual void BeginFence();
		virtual void EndFence(RHI::IRHIImmediateCommandList* immediateCommandList);

		virtual bool IsFenceCompleted() const;
		virtual bool Wait() const;
	};
}