// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12View.h"
#include "D3D12Device.h"
#include "D3D12Resource.h"

SD3D12View::SD3D12View(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12DescriptorHeap> heap, size_t resources, D3D12_DESCRIPTOR_HEAP_TYPE type) : Super(factory, device, heap.Get())
	, _heap(std::move(heap))
	, _resources(resources)
{
	_base = _heap->GetCPUDescriptorHandleForHeapStart();

	auto dev = _device->Get<ID3D12Device>();
	_incrementSize = dev->GetDescriptorHandleIncrementSize(type);
}

int32 SD3D12View::GetViewCount()
{
	return (int32)_resources.size();
}

IRHIResource* SD3D12View::GetResource(int32 indexOf)
{
	if (_resources.size() <= indexOf)
	{
		SE_LOG(LogWindows, Error, L"IndexOf is not valid. The number of resources that bound to this view is {}.", _resources.size());
		return nullptr;
	}

	if (auto ptr = _resources[indexOf].lock(); ptr)
	{
		return ptr.get();
	}
	else
	{
		SE_LOG(LogWindows, Warning, L"The resources that bound to index[{}] is expired.", indexOf);
		return nullptr;
	}
}

D3D12_CPU_DESCRIPTOR_HANDLE SD3D12View::GetHandle(int32 indexOf)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _base;
	handle.ptr += (SIZE_T)_incrementSize * indexOf;
	return handle;
}

void SD3D12View::AssignResource(int32 InIndexOf, IRHIResource* InResource)
{
	_resources[InIndexOf] = std::dynamic_pointer_cast<IRHIResource>(InResource->shared_from_this());
}