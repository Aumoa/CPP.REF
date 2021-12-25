// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12View.h"
#include "D3D12Device.h"
#include "D3D12Resource.h"

GENERATE_BODY(SD3D12View);

SD3D12View::SD3D12View(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12DescriptorHeap> Heap, size_t NumResources, D3D12_DESCRIPTOR_HEAP_TYPE HeapType)
	: Super(InFactory, InDevice, Heap.Get())
	, Heap(std::move(Heap))
	, Resources(NumResources)
{
	Base = this->Heap->GetCPUDescriptorHandleForHeapStart();

	auto Dev = Device->Get<ID3D12Device>();
	IncrementSize = Dev->GetDescriptorHandleIncrementSize(HeapType);
}

int32 SD3D12View::GetViewCount()
{
	return (int32)Resources.size();
}

IRHIResource* SD3D12View::GetResource(int32 IndexOf)
{
	if (Resources.size() <= IndexOf)
	{
		SE_LOG(LogDirectX, Error, L"IndexOf is not valid. The number of resources that bound to this view is {}.", Resources.size());
		return nullptr;
	}

	IRHIResource* Resource = Resources[IndexOf];
	if (Resource == nullptr)
	{
		SE_LOG(LogDirectX, Warning, L"The resources that bound to index[{}] is disposed.", IndexOf);
	}

	return Resource;
}

D3D12_CPU_DESCRIPTOR_HANDLE SD3D12View::GetHandle(int32 IndexOf)
{
	D3D12_CPU_DESCRIPTOR_HANDLE Handle = Base;
	Handle.ptr += (SIZE_T)IncrementSize * IndexOf;
	return Handle;
}

void SD3D12View::AssignResource(int32 IndexOf, IRHIResource* InResource)
{
	Resources[IndexOf] = InResource;
}

void SD3D12View::Dispose(bool bDisposing)
{
	Heap.Reset();

	if (bDisposing)
	{
		Resources.clear();
	}
	
	Super::Dispose(bDisposing);
}