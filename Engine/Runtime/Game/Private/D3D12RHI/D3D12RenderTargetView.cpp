// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12RenderTargetView.h"

D3D12RenderTargetView::D3D12RenderTargetView(ID3D12Resource* resource, const D3D12OfflineDescriptorIndex& handle) : Super()
	, handle(handle)
{

}

D3D12RenderTargetView::~D3D12RenderTargetView()
{
	handle.Free();
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12RenderTargetView::Handle_get() const
{
	return handle.Handle;
}