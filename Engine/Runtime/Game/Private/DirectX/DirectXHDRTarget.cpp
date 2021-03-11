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
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = { };
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	hdrBuffer = CreateRenderTexture2D(x, y, DXGI_FORMAT_B8G8R8A8_UNORM, D3D12_RESOURCE_STATE_COPY_SOURCE, false, true);
	hdrUAV->CreateUnorderedAccessView(0, hdrBuffer.Get(), nullptr, &uavDesc);
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

ID3D12Resource* DirectXHDRTarget::GetResource() const
{
	return hdrBuffer.Get();
}