// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Shaders/ColorShader/ColorVertexFactory.h"
#include "RHI/IRHIDevice.h"

SColorVertexFactory::SColorVertexFactory(IRHIDevice* device) : Super(device)
	, _device(device)
{
}

IRHIBuffer* SColorVertexFactory::CreateVertexBuffer(const RHIVertex* vertices, size_t count) const
{
	std::vector<MyVertex> vertexBuffer(count);
	for (size_t i = 0; i < count; ++i)
	{
		vertexBuffer[i] =
		{
			.Pos = vertices[i].Position,
			.Color = ((const Vector4&)vertices[i].Color).Swiz<0, 1, 2>(),
			.Normal = vertices[i].Normal
		};
	}

	RHIBufferDesc desc = {};
	desc.ByteWidth = (uint32)(sizeof(MyVertex) * vertexBuffer.size());
	desc.InitialState = ERHIResourceStates::VertexAndConstantBuffer;
	desc.Usage = ERHIBufferUsage::Immutable;

	RHISubresourceData initialData;
	initialData.pSysMem = vertexBuffer.data();
	initialData.SysMemPitch = desc.ByteWidth;

	return _device->CreateBuffer(desc, &initialData);
}

std::vector<RHIVertexElement> SColorVertexFactory::GetVertexDeclaration() const
{
	std::vector<RHIVertexElement> elements;

	// POSITION
	elements.emplace_back() =
	{
		.SemanticName = "POSITION",
		.AlignedByteOffset = 0,
		.Format = ERHIVertexElementFormat::R32G32B32_FLOAT
	};

	// COLOR
	elements.emplace_back() =
	{
		.SemanticName = "COLOR",
		.AlignedByteOffset = 12,
		.Format = ERHIVertexElementFormat::R32G32B32_FLOAT
	};

	// NORMAL
	elements.emplace_back() =
	{
		.SemanticName = "NORMAL",
		.AlignedByteOffset = 24,
		.Format = ERHIVertexElementFormat::R32G32B32_FLOAT
	};

	return elements;
}