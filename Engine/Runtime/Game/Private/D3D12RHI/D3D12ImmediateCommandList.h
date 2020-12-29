// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "D3D12DeferredCommandList.h"
#include "RHI/IRHIImmediateCommandList.h"

#include "D3D12Minimal.h"

class D3D12ImmediateCommandList : public D3D12DeferredCommandList, virtual public IRHIImmediateCommandList
{
public:
	using Super = D3D12DeferredCommandList;
	using This = D3D12ImmediateCommandList;

private:
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12CommandQueue> commandQueue;

public:
	D3D12ImmediateCommandList(ID3D12Device* device);
	~D3D12ImmediateCommandList() override;

	virtual void ExecuteCommandList(IRHIDeferredCommandList* deferredCommandList);
	virtual void Flush();

	vs_property_get(ID3D12CommandQueue*, CommandQueue);
	ID3D12CommandQueue* CommandQueue_get() const;
};