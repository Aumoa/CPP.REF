// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXDescriptorAllocator.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXShaderResourceView.h"

DirectXDescriptorAllocator::DirectXDescriptorAllocator(DirectXDeviceBundle* deviceBundle) : Super()
	, device(deviceBundle->GetDevice())
	, numDescriptors(0)
	, issued(0)

	, handleStart(0)
	, handleStartGpu(0)
	, incrementSize(0)
{
	SetDeviceChildPtr(nullptr, deviceBundle);
}

DirectXDescriptorAllocator::~DirectXDescriptorAllocator()
{

}

void DirectXDescriptorAllocator::SetDescriptorHeaps(ID3D12GraphicsCommandList4* inCommandList)
{
	inCommandList->SetDescriptorHeaps(1, descriptorHeap.GetAddressOf());
}

void DirectXDescriptorAllocator::BeginAllocate(uint32 capacity)
{
	if (numDescriptors < capacity)
	{
		Capacity = capacity;
	}

	issued = 0;
}

void DirectXDescriptorAllocator::EndAllocate()
{

}

uint32 DirectXDescriptorAllocator::Issue(DirectXShaderResourceView* inSRV)
{
	uint32 issueCount = inSRV->NumDescriptors;

	D3D12_CPU_DESCRIPTOR_HANDLE src = inSRV->GetCPUHandle(0);
	D3D12_CPU_DESCRIPTOR_HANDLE dst = { handleStart + (size_t)(issued * incrementSize) };

	device->CopyDescriptorsSimple(issueCount, dst, src, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	uint32 ret = issued;
	issued += issueCount;
	return ret;
}

D3D12_GPU_DESCRIPTOR_HANDLE DirectXDescriptorAllocator::GetGPUHandle(uint32 issueNumber) const
{
	return { handleStartGpu + (uint64)(incrementSize * issueNumber) };
}

uint32 DirectXDescriptorAllocator::Capacity_get() const
{
	return numDescriptors;
}

void DirectXDescriptorAllocator::Capacity_set(uint32 value)
{
	if (numDescriptors == value)
	{
		return;
	}

	D3D12_DESCRIPTOR_HEAP_DESC desc = { };
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = value;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HR(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

	numDescriptors = value;
	handleStart = descriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr;
	handleStartGpu = descriptorHeap->GetGPUDescriptorHandleForHeapStart().ptr;
	incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}