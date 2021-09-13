// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Shaders/ColorShader/ColorVertexFactory.h"

SColorVertexFactory::SColorVertexFactory(SRHIDevice* device) : Super(device)
{
}

SRHIResource* SColorVertexFactory::CreateVertexBuffer(const RHIVertex* vertices, size_t count) const
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

	return GetDevice()->CreateImmutableBuffer(ERHIResourceStates::VertexAndConstantBuffer, (const uint8*)vertexBuffer.data(), std::span(vertexBuffer).size_bytes());
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