// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12RenderTargetView.h"

#include "Engine.h"
#include "D3D12DeviceBundle.h"

D3D12RenderTargetView::D3D12RenderTargetView(ID3D12Resource* resource, const D3D12DescriptorIndex& handle) : Super()
	, handle(handle)
{

}

D3D12RenderTargetView::~D3D12RenderTargetView()
{

}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12RenderTargetView::Handle_get() const
{
	return handle.Handle;
}