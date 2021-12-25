// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectXCommon.h"

class DIRECTX_API SD3D12DescriptorHeap : implements SObject
{
	GENERATED_BODY(SD3D12DescriptorHeap)

private:
	ID3D12Device* Device = nullptr;
	int32 Count = 0;
	ComPtr<ID3D12DescriptorHeap> Heap;

	uint32 IncrementSize = 0;
	int32 Index = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE HandleCPU = {};
	D3D12_GPU_DESCRIPTOR_HANDLE HandleGPU = {};

public:
	SD3D12DescriptorHeap(ID3D12Device* InDevice, int32 Count);

	void MarkPendingHeap();

	int32 Increment(int32 Count = 1);
	int32 Slack();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(std::optional<int32> InIndex = std::nullopt);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(std::optional<int32> InIndex = std::nullopt);

protected:
	virtual void Dispose(bool bDisposing) override;

public:
	DECLARE_GETTER(ID3D12DescriptorHeap, Heap);
};