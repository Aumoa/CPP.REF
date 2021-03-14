// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXDeferredContext.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"

DirectXDeferredContext::DirectXDeferredContext(DirectXDeviceBundle* deviceBundle, D3D12_COMMAND_LIST_TYPE type) : Super(deviceBundle, type)
{
	ID3D12Device5* device = deviceBundle->GetDevice();
	HR(device->CreateCommandAllocator(type, IID_PPV_ARGS(&swapAllocator)));
}

DirectXDeferredContext::~DirectXDeferredContext()
{

}

void DirectXDeferredContext::BeginDraw()
{
	SwapCommandAllocator(swapAllocator);
	Super::BeginDraw();
}

void DirectXDeferredContext::DebugName_set(TRefPtr<String> value)
{
	swapAllocator->SetName(value->C_Str);
}