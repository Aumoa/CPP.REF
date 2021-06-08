// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Core;
import SC.Runtime.Game.Shaders;
import SC.Runtime.RenderCore;
import std.core;

#define BYTE uint8

#include "ColorShaderVS.hlsl.h"
#include "ColorShaderPS.hlsl.h"

using namespace std;

ColorShader::ColorShader(RHIDevice* device) : Super(device)
{
}

RHIResource* ColorShader::CreateVertexBuffer(const RHIVertex* vertices, size_t count) const
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

	return GetDevice()->CreateImmutableBuffer(ERHIResourceStates::VertexAndConstantBuffer, (uint8*)vertexBuffer.data(), span(vertexBuffer).size_bytes());
}

span<uint8 const> ColorShader::CompileVS()
{
	return pColorShaderVS;
}

span<uint8 const> ColorShader::CompilePS()
{
	return pColorShaderPS;
}

vector<RHIVertexElement> ColorShader::GetVertexDeclaration() const
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