// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXView.h"
#include "DirectXDevice.h"

GENERATE_BODY(SDirectXView);

SDirectXView::SDirectXView(SDirectXDevice* Owner, ComPtr<ID3D12DescriptorHeap> pDescriptorHeap, size_t NumViews)
	: Super(Owner)
	, pDescriptorHeap(std::move(pDescriptorHeap))
{
	BindResources.resize(NumViews);
	IncrementSize = Owner->pDevice->GetDescriptorHandleIncrementSize(this->pDescriptorHeap->GetDesc().Type);
}

int32 SDirectXView::GetViewCount()
{
	return (int32)BindResources.size();
}

IRHIResource* SDirectXView::GetResource(int32 IndexOf)
{
	return BindResources[IndexOf];
}

D3D12_CPU_DESCRIPTOR_HANDLE SDirectXView::GetCPUHandle(int32 IndexOf)
{
	if (!BaseCPUHandle.has_value())
	{
		BaseCPUHandle = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE Base = *BaseCPUHandle;
	Base.ptr += (uint32)IndexOf * IncrementSize;
	return Base;
}

D3D12_GPU_DESCRIPTOR_HANDLE SDirectXView::GetGPUHandle(int32 IndexOf)
{
	if (!BaseGPUHandle.has_value())
	{
		BaseGPUHandle = pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	}

	D3D12_GPU_DESCRIPTOR_HANDLE Base = *BaseGPUHandle;
	Base.ptr += (uint32)IndexOf * IncrementSize;
	return Base;
}

void SDirectXView::Dispose(bool bDisposing)
{
	pDescriptorHeap.Reset();
	BindResources.clear();

	Super::Dispose(bDisposing);
}