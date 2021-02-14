// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIOnlineDescriptorPatch.h"

#include "D3D12Minimal.h"

class D3D12OnlineDescriptorPatch : virtual public Object, virtual public IRHIOnlineDescriptorPatch
{
public:
	using Super = Object;
	using This = D3D12OnlineDescriptorPatch;

private:
	ID3D12Device* device;

	ComPtr<ID3D12DescriptorHeap> heap;
	size_t count;
	size_t seekpos;

	CD3DX12_CPU_DESCRIPTOR_HANDLE handle_seek;
	uint32 increment;
	CD3DX12_GPU_DESCRIPTOR_HANDLE handle_start;

public:
	D3D12OnlineDescriptorPatch();
	~D3D12OnlineDescriptorPatch() override;

	void ReallocAndStart(size_t num);
	size_t Patch(IRHIShaderResourceView* srv);

	D3D12_GPU_DESCRIPTOR_HANDLE GetOnlineHandle(size_t seekpos) const;

	vs_property_get_auto(ID3D12DescriptorHeap*, Heap, heap.Get());
};