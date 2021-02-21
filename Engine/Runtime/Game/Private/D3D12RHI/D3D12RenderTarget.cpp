// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12RenderTarget.h"

#include "D3D12Resource.h"

D3D12RenderTarget::D3D12RenderTarget() : Super()
{

}

D3D12RenderTarget::~D3D12RenderTarget()
{

}

TRefPtr<D3D12Resource> D3D12RenderTarget::CreateTexture2D(ID3D12Device* device, DXGI_FORMAT format, D3D12_RESOURCE_STATES initialState, int32 width, int32 height, bool bAllowUnorderedAccess)
{
	D3D12_RESOURCE_DESC textureDesc = { };
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Width = (uint64)width;
	textureDesc.Height = (uint32)height;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc = { 1, 0 };
	textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	if (bAllowUnorderedAccess)
	{
		textureDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}

	D3D12_HEAP_PROPERTIES heapProp = { };
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_CLEAR_VALUE clearValue = { };
	clearValue.Format = format;

	// Is depth stencil buffer format.
	// IMPORTANT WARNING: Internal only!!
	if (format == DXGI_FORMAT_R24G8_TYPELESS)
	{
		textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		clearValue.DepthStencil.Depth = 1.0f;
	}

	ComPtr<ID3D12Resource> resource;
	HR(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &textureDesc, initialState, &clearValue, IID_PPV_ARGS(&resource)));

	return NewObject<D3D12Resource>(resource.Get());
}