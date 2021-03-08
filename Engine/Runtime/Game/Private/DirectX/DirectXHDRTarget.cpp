// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXHDRTarget.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXShaderResourceView.h"

DirectXHDRTarget::DirectXHDRTarget(DirectXDeviceBundle* deviceBundle) : Super(deviceBundle)
{
	DirectXNew(hdrUAV, DirectXShaderResourceView, deviceBundle, 1);
	DirectXNew(hdrSRV, DirectXShaderResourceView, deviceBundle, 1);
}

DirectXHDRTarget::~DirectXHDRTarget()
{

}

void DirectXHDRTarget::ResizeBuffers(int32 x, int32 y)
{
	hdrBuffer = CreateRenderTexture2D(x, y, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, false, true);
	hdrUAV->CreateUnorderedAccessView(0, hdrBuffer.Get(), nullptr, nullptr);
	hdrSRV->CreateShaderResourceView(0, hdrBuffer.Get(), nullptr);
}

DirectXShaderResourceView* DirectXHDRTarget::GetUAV() const
{
	return hdrUAV.Get();
}

DirectXShaderResourceView* DirectXHDRTarget::GetSRV() const
{
	return hdrSRV.Get();
}