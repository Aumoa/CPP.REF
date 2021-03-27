// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXDepthStencilView.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "Logging/LogMacros.h"

DirectXDepthStencilView::DirectXDepthStencilView(DirectXDeviceBundle* deviceBundle, uint32 numRTVs) : Super(deviceBundle, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, numRTVs)
	, device(deviceBundle->GetDevice())
{

}

DirectXDepthStencilView::~DirectXDepthStencilView()
{

}

void DirectXDepthStencilView::CreateView(size_t index, ID3D12Resource* rtvTarget, const D3D12_DEPTH_STENCIL_VIEW_DESC* rtvDesc)
{
	if (index > NumDescriptors)
	{
		SE_LOG(LogDirectX, Error, L"CreateView: index is over than NumDescriptors.");
		return;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE handle = GetCPUHandle(index);
	device->CreateDepthStencilView(rtvTarget, rtvDesc, handle);
}