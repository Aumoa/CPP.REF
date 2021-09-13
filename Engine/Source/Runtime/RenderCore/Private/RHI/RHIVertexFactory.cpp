// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHI/RHIVertexFactory.h"
#include "RHI/RHIDevice.h"

SRHIVertexFactory::SRHIVertexFactory(SRHIDevice* device) : Super(device)
{
}

SRHIResource* SRHIVertexFactory::CreateVertexBuffer(const RHIVertex* vertices, size_t count) const
{
	SRHIDevice* dev = GetDevice();
	return dev->CreateImmutableBuffer(ERHIResourceStates::VertexAndConstantBuffer, (const uint8*)vertices, sizeof(RHIVertex) * count);
}

SRHIResource* SRHIVertexFactory::CreateIndexBuffer(const uint32* indices, size_t count) const
{
	SRHIDevice* dev = GetDevice();
	return dev->CreateImmutableBuffer(ERHIResourceStates::IndexBuffer, (const uint8*)indices, sizeof(uint32) * count);
}