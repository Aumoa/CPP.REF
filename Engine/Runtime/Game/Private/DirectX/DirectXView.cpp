// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXView.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "Logging/LogMacros.h"

DirectXView::DirectXView(DirectXDeviceBundle* deviceBundle, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32 numDescriptors, bool bShaderVisible) : Super()
	, numDescriptors(numDescriptors)
	, bShaderVisible(bShaderVisible)

	, incrementSize(0)
	, handleCPU(0)
	, handleGPU(0)
{
	ID3D12Device5* device = deviceBundle->GetDevice();
	
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = { };
	heapDesc.Type = heapType;
	heapDesc.NumDescriptors = numDescriptors;
	heapDesc.Flags = bShaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HR(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&descriptorHeap)));

	incrementSize = device->GetDescriptorHandleIncrementSize(heapType);
	handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr;
	if (bShaderVisible)
	{
		handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart().ptr;
	}
}

DirectXView::~DirectXView()
{

}

D3D12_CPU_DESCRIPTOR_HANDLE DirectXView::GetCPUHandle(size_t index) const
{
	return D3D12_CPU_DESCRIPTOR_HANDLE{ handleCPU + (size_t)incrementSize * index };
}

D3D12_GPU_DESCRIPTOR_HANDLE DirectXView::GetGPUHandle(size_t index) const
{
	if (!bShaderVisible)
	{
		SE_LOG(LogDirectX, Error, L"GPU Descriptor handle exists only when descriptor is shader visible.");
		return { };
	}

	return D3D12_GPU_DESCRIPTOR_HANDLE{ handleGPU + (uint64)incrementSize * (uint64)index };
}