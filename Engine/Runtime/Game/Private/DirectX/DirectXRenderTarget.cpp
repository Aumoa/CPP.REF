// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXRenderTarget.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"

DirectXRenderTarget::DirectXRenderTarget(DirectXDeviceBundle* deviceBundle) : Super()
	, device(deviceBundle->GetDevice())
{
	SetDeviceChildPtr(nullptr, deviceBundle);
}

DirectXRenderTarget::~DirectXRenderTarget()
{

}

ID3D12Device5* DirectXRenderTarget::GetDevice() const
{
	return device;
}

TComPtr<ID3D12Resource> DirectXRenderTarget::CreateRenderTexture2D(uint32 width, uint32 height, DXGI_FORMAT format, D3D12_RESOURCE_STATES initialState, bool bAllowRenderTarget, bool bAllowUnorderedAccess)
{
	D3D12_RESOURCE_DESC textureDesc = { };
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Width = (uint64)width;
	textureDesc.Height = height;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc = { 1, 0 };
	textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	
	if (bAllowRenderTarget)
	{
		textureDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	}

	if (bAllowUnorderedAccess)
	{
		textureDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}

	// Heap type is must be DEFAULT.
	D3D12_HEAP_PROPERTIES heapProp = { };
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	// Clear value should be only use for render target.
	D3D12_CLEAR_VALUE clearValue = { };
	if (bAllowRenderTarget)
	{
		clearValue.Format = format;
	}

	// Create commonly committed resource.
	TComPtr<ID3D12Resource> resource;
	HR(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &textureDesc, initialState, bAllowRenderTarget ? &clearValue : nullptr, IID_PPV_ARGS(&resource)));

	return resource;
}

TComPtr<ID3D12Resource> DirectXRenderTarget::CreateDepthStencilTexture2D(uint32 width, uint32 height, DXGI_FORMAT format)
{
	D3D12_RESOURCE_DESC textureDesc = { };
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Width = (uint64)width;
	textureDesc.Height = height;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc = { 1, 0 };
	textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	// Heap type is must be DEFAULT.
	D3D12_HEAP_PROPERTIES heapProp = { };
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	// Clear value should be only use for render target.
	D3D12_CLEAR_VALUE clearValue = { };
	clearValue.Format = format;
	clearValue.DepthStencil.Depth = 1.0f;

	// Create commonly committed resource.
	TComPtr<ID3D12Resource> resource;
	HR(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, IID_PPV_ARGS(&resource)));

	return resource;
}