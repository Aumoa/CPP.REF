// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Windows/WindowsRHIDescriptorTable.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHIDevice.h"
#include "RHI/Windows/WindowsRHIShaderResourceViewTable.h"

std::shared_ptr<RHIDescriptorTable> WindowsRHIDevice::CreateDescriptorTable(size_t descriptors)
{
	return MakeShared<WindowsRHIDescriptorTable>(SharedFromThis(), descriptors);
}

WindowsRHIDescriptorTable::WindowsRHIDescriptorTable(std::shared_ptr<WindowsRHIDevice> device, size_t descriptors)
	: RHIDescriptorTable(device)
	, _descriptors(descriptors)
{
	_dev = WinGetr(device);
	D3D12_DESCRIPTOR_HEAP_DESC desc =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		.NumDescriptors = (UINT)descriptors,
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
	};
	HR(_dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_heap)));
	_incr = _dev->GetDescriptorHandleIncrementSize(desc.Type);
}

WindowsRHIDescriptorTable::~WindowsRHIDescriptorTable() noexcept
{
}

size_t WindowsRHIDescriptorTable::GetMaxDescriptors() const
{
	return _descriptors;
}

uint64 WindowsRHIDescriptorTable::CopyFrom(size_t index, RHIShaderResourceViewTable* viewTable, size_t viewIndex, size_t viewCount)
{
	auto* vHeap = WinGetr(viewTable);

	D3D12_CPU_DESCRIPTOR_HANDLE copyFrom = vHeap->GetCPUDescriptorHandleForHeapStart();
	copyFrom.ptr += _incr * viewIndex;

	D3D12_CPU_DESCRIPTOR_HANDLE copyTo = _heap->GetCPUDescriptorHandleForHeapStart();
	copyTo.ptr += _incr * index;

	_dev->CopyDescriptorsSimple((UINT)viewCount, copyTo, copyFrom, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	D3D12_GPU_DESCRIPTOR_HANDLE heapPtr = _heap->GetGPUDescriptorHandleForHeapStart();
	return heapPtr.ptr + _incr * index;
}

uint64 WindowsRHIDescriptorTable::GetDescriptor(size_t index) const
{
	D3D12_GPU_DESCRIPTOR_HANDLE heapPtr = _heap->GetGPUDescriptorHandleForHeapStart();
	return heapPtr.ptr + _incr * index;
}

#endif