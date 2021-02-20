// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHICommandFence.h"

#include "D3D12Minimal.h"

class D3D12CommandFence : virtual public Object, virtual public IRHICommandFence
{
public:
	using Super = Object;

private:
	ComPtr<ID3D12Fence> fence;
	uint64 fenceValue;
	HANDLE hEvent;

public:
	D3D12CommandFence(ID3D12Device* device);
	~D3D12CommandFence() override;

	virtual void BeginFence();
	virtual void EndFence(IRHIImmediateCommandList* immediateCommandList);

	virtual bool IsFenceCompleted() const;
	virtual bool Wait() const;
};