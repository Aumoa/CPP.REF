// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHIVertexFactory.h"
#include "RHIDevice.h"

RHIVertexFactory::RHIVertexFactory(RHIDevice* device) : Super(device)
{
}

RHIResource* RHIVertexFactory::CreateVertexBuffer(const RHIVertex* vertices, size_t count) const
{
	RHIDevice* dev = GetDevice();
	return dev->CreateImmutableBuffer(ERHIResourceStates::VertexAndConstantBuffer, (const uint8*)vertices, sizeof(RHIVertex) * count);
}

RHIResource* RHIVertexFactory::CreateIndexBuffer(const uint32* indices, size_t count) const
{
	RHIDevice* dev = GetDevice();
	return dev->CreateImmutableBuffer(ERHIResourceStates::IndexBuffer, (const uint8*)indices, sizeof(uint32) * count);
}