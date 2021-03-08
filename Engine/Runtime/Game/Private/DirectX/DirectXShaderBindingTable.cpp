// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXShaderBindingTable.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"

DirectXShaderBindingTable::DirectXShaderBindingTable(DirectXDeviceBundle* deviceBundle) : Super()
	, device(deviceBundle->GetDevice())
{
	rayGeneration = CreateBuffer(D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
}

DirectXShaderBindingTable::~DirectXShaderBindingTable()
{

}

TComPtr<ID3D12Resource> DirectXShaderBindingTable::CreateBuffer(uint64 sizeInBytes)
{
	D3D12_RESOURCE_DESC bufferDesc = { };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = sizeInBytes;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_HEAP_PROPERTIES heapProp = { };
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	TComPtr<ID3D12Resource> resource;
	HR(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource)));

	return resource;
}