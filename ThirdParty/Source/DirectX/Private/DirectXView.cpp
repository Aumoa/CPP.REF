// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXView.h"
#include "DirectXDevice.h"

using namespace ::libty;

SDirectXView::SDirectXView(SDirectXDevice* Owner, ComPtr<ID3D12DescriptorHeap> pDescriptorHeap, size_t NumViews)
	: Super(Owner)
	, pDescriptorHeap(std::move(pDescriptorHeap))
{
	BindResources.resize(NumViews);
	IncrementSize = Owner->pDevice->GetDescriptorHandleIncrementSize(this->pDescriptorHeap->GetDesc().Type);
}

size_t SDirectXView::GetViewCount()
{
	return BindResources.size();
}

IRHIResource* SDirectXView::GetResource(size_t indexOf)
{
	return BindResources[indexOf];
}

D3D12_CPU_DESCRIPTOR_HANDLE SDirectXView::GetCPUHandle(size_t indexOf)
{
	if (!BaseCPUHandle.has_value())
	{
		BaseCPUHandle = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE Base = *BaseCPUHandle;
	Base.ptr += (SIZE_T)indexOf * IncrementSize;
	return Base;
}

D3D12_GPU_DESCRIPTOR_HANDLE SDirectXView::GetGPUHandle(size_t indexOf)
{
	if (!BaseGPUHandle.has_value())
	{
		BaseGPUHandle = pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	}

	D3D12_GPU_DESCRIPTOR_HANDLE Base = *BaseGPUHandle;
	Base.ptr += (UINT64)indexOf * IncrementSize;
	return Base;
}

void SDirectXView::Dispose(bool bDisposing)
{
	pDescriptorHeap.Reset();
	BindResources.clear();

	Super::Dispose(bDisposing);
}