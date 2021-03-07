// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXDeviceContext.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"

DirectXDeviceContext::DirectXDeviceContext(DirectXDeviceBundle* deviceBundle, D3D12_COMMAND_LIST_TYPE type) : Super()
{
	ID3D12Device5* device = deviceBundle->GetDevice();
	HR(device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)));
}

DirectXDeviceContext::~DirectXDeviceContext()
{

}

void DirectXDeviceContext::BeginDraw()
{

}

void DirectXDeviceContext::EndDraw()
{

}

ID3D12GraphicsCommandList4* DirectXDeviceContext::GetCommandList() const
{
	return commandList.Get();
}

void DirectXDeviceContext::SwapCommandAllocator(TComPtr<ID3D12CommandAllocator>& swapTarget)
{
	commandAllocator.Swap(swapTarget);
}