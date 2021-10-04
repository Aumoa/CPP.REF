// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12View.h"
#include "D3D12Device.h"

SD3D12View::SD3D12View(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12DescriptorHeap> heap, size_t resources, D3D12_DESCRIPTOR_HEAP_TYPE type) : Super(factory, device)
	, _heap(std::move(heap))
	, _resources(resources)
{
	_base = _heap->GetCPUDescriptorHandleForHeapStart();

	auto dev = _device->Get<ID3D12Device>();
	_incrementSize = dev->GetDescriptorHandleIncrementSize(type);
}

IRHIResource* SD3D12View::GetResource(int32 indexOf)
{
	if (_resources.size() <= indexOf)
	{
		SE_LOG(LogDirectX, Error, L"IndexOf is not valid. The number of resources that bound to this view is {}.", _resources.size());
		return nullptr;
	}

	return _resources[indexOf].get();
}

D3D12_CPU_DESCRIPTOR_HANDLE SD3D12View::GetHandle(int32 indexOf)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _base;
	handle.ptr += (SIZE_T)_incrementSize * indexOf;
	return handle;
}