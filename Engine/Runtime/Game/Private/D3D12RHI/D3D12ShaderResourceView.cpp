// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12ShaderResourceView.h"

D3D12ShaderResourceView::D3D12ShaderResourceView(ID3D12Resource* resource, const D3D12OnlineDescriptorIndex& handle) : Super()
	, handle(handle)
{

}

D3D12ShaderResourceView::~D3D12ShaderResourceView()
{
	handle.Free();
}

D3D12_GPU_DESCRIPTOR_HANDLE D3D12ShaderResourceView::Handle_get() const
{
	return handle.HandleGPU;
}