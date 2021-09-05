// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHI/RHIShaderDescriptorView.h"
#include "RHI/LogRHI.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHIDeviceContext.h"
#include "RHI/RHIShaderResourceView.h"
#include "InternalComPtr.h"

RHIShaderDescriptorView::RHIShaderDescriptorView(RHIDevice* device) : Super(device)
{
}

RHIShaderDescriptorView::~RHIShaderDescriptorView()
{
}

void RHIShaderDescriptorView::SetMaxDescriptorCount(size_t count)
{
	const bool bRealloc = !_descriptorHeap || count > _count;

	if (bRealloc)
	{
		if (_descriptorHeap && _bIsCommit)
		{
			// Add to pending descriptor.
			check(!_pendingDestroyHeap);
			_pendingDestroyHeap = _descriptorHeap;
		}

		ID3D12Device* const device = GetDevice()->GetDevice();
		D3D12_DESCRIPTOR_HEAP_DESC desc =
		{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			.NumDescriptors = std::max((UINT)count, (UINT)1),
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
			.NodeMask = 0
		};

		HR(LogRHI, device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descriptorHeap)));
		_incrementSize = device->GetDescriptorHandleIncrementSize(desc.Type);
	}
}

size_t RHIShaderDescriptorView::GetMaxDescriptorCount() const
{
	return _count;
}

void RHIShaderDescriptorView::ResetBindings()
{
	_seekpos = 0;
}

size_t RHIShaderDescriptorView::Bind(RHIShaderResourceView* view)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleSrc = view->GetCPUDescriptorHandle(0);
	size_t count = view->GetDescriptorCount();

	D3D12_CPU_DESCRIPTOR_HANDLE handleDst = GetCPUDescriptorHandle(_seekpos);

	ID3D12Device* device = GetDevice()->GetDevice();
	device->CopyDescriptorsSimple((UINT)count, handleDst, handleSrc, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	size_t indexDst = _seekpos;
	_seekpos += count;
	return indexDst;
}

D3D12_CPU_DESCRIPTOR_HANDLE RHIShaderDescriptorView::GetCPUDescriptorHandle(size_t index) const
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleBase = _descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleBase.ptr += (size_t)_incrementSize * index;
	return handleBase;
}

D3D12_GPU_DESCRIPTOR_HANDLE RHIShaderDescriptorView::GetGPUDescriptorHandle(size_t index) const
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleBase = _descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleBase.ptr += (UINT64)_incrementSize * index;
	return handleBase;
}