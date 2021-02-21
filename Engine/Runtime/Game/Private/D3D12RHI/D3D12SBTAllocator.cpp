// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12SBTAllocator.h"

#include "D3D12Shader.h"

D3D12SBTAllocator::D3D12SBTAllocator(ID3D12Device5* device, D3D12Shader* inShader) : Super()
{
	D3D12_RESOURCE_DESC bufferDesc = { };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_HEAP_PROPERTIES heapProp = { D3D12_HEAP_TYPE_UPLOAD };

	HR(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&rayGeneration)));

	D3D12ShaderBindingParam* param = nullptr;
	HR(rayGeneration->Map(0, nullptr, (void**)&param));
	memcpy(param->Identifier, inShader->pShaderIdentifier, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
}

D3D12SBTAllocator::~D3D12SBTAllocator()
{

}

uint64 D3D12SBTAllocator::RayGenerationStartAddress_get() const
{
	return rayGeneration->GetGPUVirtualAddress();
}

uint64 D3D12SBTAllocator::RayGenerationBufferStride_get() const
{
	return D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
}