// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXShaderBindingTable.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXRaytracingShader.h"

namespace
{
	extern "C" struct DXRayGenerationBinding
	{
		char Identifier[D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES];
		D3D12_GPU_DESCRIPTOR_HANDLE rootParameters[1];
	};
}

DirectXShaderBindingTable::DirectXShaderBindingTable(DirectXDeviceBundle* deviceBundle) : Super()
	, device(deviceBundle->GetDevice())
{
	rayGeneration = CreateBuffer(D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	SetDeviceChildPtr(nullptr, deviceBundle);
}

DirectXShaderBindingTable::~DirectXShaderBindingTable()
{

}

void DirectXShaderBindingTable::Init(DirectXRaytracingShader* initShader)
{
	DXRayGenerationBinding* rgBind;
	HR(rayGeneration->Map(0, nullptr, (void**)&rgBind));
	memcpy(rgBind->Identifier, initShader->GetRayGenerationIdentifier(), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	rayGeneration->Unmap(0, nullptr);
}

void DirectXShaderBindingTable::FillDispatchRaysDesc(D3D12_DISPATCH_RAYS_DESC& outDispatchRays) const
{
	outDispatchRays.RayGenerationShaderRecord.StartAddress = rayGeneration->GetGPUVirtualAddress();
	outDispatchRays.RayGenerationShaderRecord.SizeInBytes = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
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