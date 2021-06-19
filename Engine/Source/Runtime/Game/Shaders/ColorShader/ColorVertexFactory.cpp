// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ColorVertexFactory.h"
#include "RHI/RHIDevice.h"

using namespace std;

ColorVertexFactory::ColorVertexFactory(RHIDevice* device) : Super(device)
{
}

RHIResource* ColorVertexFactory::CreateVertexBuffer(const RHIVertex* vertices, size_t count) const
{
	vector<MyVertex> vertexBuffer(count);
	for (size_t i = 0; i < count; ++i)
	{
		vertexBuffer[i] =
		{
			.Pos = vertices[i].Position,
			.Color = ((const Vector4&)vertices[i].Color).Swiz<0, 1, 2>()
		};
	}

	return GetDevice()->CreateImmutableBuffer(ERHIResourceStates::VertexAndConstantBuffer, (const uint8*)vertexBuffer.data(), span(vertexBuffer).size_bytes());
}

vector<RHIVertexElement> ColorVertexFactory::GetVertexDeclaration() const
{
	vector<RHIVertexElement> elements;

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

	return elements;
}