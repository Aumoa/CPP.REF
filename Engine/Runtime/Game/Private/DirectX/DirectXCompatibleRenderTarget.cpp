// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXCompatibleRenderTarget.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXRenderTargetView.h"

DirectXCompatibleRenderTarget::DirectXCompatibleRenderTarget(DirectXDeviceBundle* deviceBundle) : Super(deviceBundle)
{
	DirectXNew(compatibleRTV, DirectXRenderTargetView, deviceBundle, 1);
}

DirectXCompatibleRenderTarget::~DirectXCompatibleRenderTarget()
{

}

void DirectXCompatibleRenderTarget::ResizeBuffers(int32 x, int32 y)
{
	compatibleBuffer = CreateRenderTexture2D(x, y, DXGI_FORMAT_B8G8R8A8_UNORM, D3D12_RESOURCE_STATE_COPY_SOURCE, true, false);
	compatibleRTV->CreateView(0, compatibleBuffer.Get(), nullptr);
}

DirectXRenderTargetView* DirectXCompatibleRenderTarget::GetRTV() const
{
	return compatibleRTV.Get();
}