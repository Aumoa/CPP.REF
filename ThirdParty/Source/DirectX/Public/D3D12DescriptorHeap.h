// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectXCommon.h"

class DIRECTX_API SD3D12DescriptorHeap : implements SObject
{
	GENERATED_BODY(SD3D12DescriptorHeap)

private:
	ID3D12Device* _device = nullptr;
	int32 _count = 0;
	ComPtr<ID3D12DescriptorHeap> _heap;

	uint32 _incrementSize = 0;
	int32 _index = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE _handleCPU = {};
	D3D12_GPU_DESCRIPTOR_HANDLE _handleGPU = {};

public:
	SD3D12DescriptorHeap(ID3D12Device* device, int32 count);

	void MarkPendingHeap();

	int32 Increment(int32 count = 1);
	int32 Slack();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(std::optional<int32> index = std::nullopt);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(std::optional<int32> index = std::nullopt);

public:
	DECLARE_GETTER(ID3D12DescriptorHeap, _heap);
};