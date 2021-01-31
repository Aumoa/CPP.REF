// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12DepthStencilView.h"

#include "Engine.h"
#include "D3D12DeviceBundle.h"

D3D12DepthStencilView::D3D12DepthStencilView(ID3D12Resource* resource, const D3D12DescriptorIndex& handle) : Super()
, handle(handle)
{

}

D3D12DepthStencilView::~D3D12DepthStencilView()
{

}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12DepthStencilView::Handle_get() const
{
	return handle.Handle;
}