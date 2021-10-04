// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "VertexFactory.h"
#include "RHI/IRHIDevice.h"

SVertexFactory::SVertexFactory(IRHIDevice* device) : Super()
	, _device(device)
{
}

IRHIBuffer* SVertexFactory::CreateVertexBuffer(const RHIVertex* vertices, size_t count) const
{
	RHIBufferDesc bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(RHIVertex) * count;
	bufferDesc.InitialState = ERHIResourceStates::VertexAndConstantBuffer;
	bufferDesc.Usage = ERHIBufferUsage::Immutable;
	
	RHISubresourceData subresource = {};
	subresource.pSysMem = vertices;

	IRHIBuffer* buffer = _device->CreateBuffer(bufferDesc, &subresource);
	return buffer;
}

IRHIBuffer* SVertexFactory::CreateIndexBuffer(const uint32* indices, size_t count) const
{
	RHIBufferDesc bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(RHIVertex) * count;
	bufferDesc.InitialState = ERHIResourceStates::IndexBuffer;
	bufferDesc.Usage = ERHIBufferUsage::Immutable;

	RHISubresourceData subresource = {};
	subresource.pSysMem = indices;

	IRHIBuffer* buffer = _device->CreateBuffer(bufferDesc, &subresource);
	return buffer;
}