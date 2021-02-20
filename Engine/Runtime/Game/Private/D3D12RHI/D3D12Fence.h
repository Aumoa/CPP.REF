// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "D3D12Minimal.h"
#include "RHI/IRHIFence.h"

class D3D12Fence : virtual public Object, virtual public IRHIFence
{
public:
	using Super = Object;
	using This = D3D12Fence;

private:
	ComPtr<ID3D12Fence> fence;

public:
	D3D12Fence();
	~D3D12Fence() override;

	virtual uint64 GetCompletedValue() const;
	virtual void Signal(IRHIImmediateCommandList* queue, uint64 number);
};