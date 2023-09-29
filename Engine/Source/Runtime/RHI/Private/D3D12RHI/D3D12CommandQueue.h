// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHICommandQueue.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12Graphics;

class ND3D12CommandQueue : public NRHICommandQueue
{
	ComPtr<ID3D12CommandQueue> Queue;

public:
	ND3D12CommandQueue(ComPtr<ID3D12CommandQueue> pQueue);
	virtual ~ND3D12CommandQueue() noexcept override;

	virtual void ExecuteCommandSets(std::span<NRHICommandSet const* const> InCommandSets) override;
	
	ID3D12CommandQueue* GetQueue() const { return Queue.Get(); }
};

#endif