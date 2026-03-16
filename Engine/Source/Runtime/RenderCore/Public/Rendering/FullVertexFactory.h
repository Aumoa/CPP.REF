// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/VertexFactory.h"
#include "Rendering/InputElementDescription.h"
#include "Rendering/InputElementSemantic.h"
#include "Rendering/GraphicsFormat.h"
#include "FullVertexFactory.gen.h"

namespace Ayla
{
	/// <summary>
	/// Vertex factory for full vertex data (Position + Normal + Tangent + Bitangent + TexCoord).
	/// Used for PBR rendering with normal mapping.
	/// </summary>
	ACLASS()
	class RENDERCORE_API FullVertexFactory : public VertexFactory
	{
		GENERATED_BODY()

	public:
		ACONSTRUCTOR()
		FullVertexFactory() = default;

		virtual std::vector<InputElementDescription> GetInputElements() const override
		{
			return
			{
				InputElementDescription(InputElementSemantic::Position,  0, GraphicsFormat::R32G32B32_Float, 0,  0, 0),
				InputElementDescription(InputElementSemantic::Normal,    0, GraphicsFormat::R32G32B32_Float, 0, 12, 0),
				InputElementDescription(InputElementSemantic::Tangent,   0, GraphicsFormat::R32G32B32_Float, 0, 24, 0),
				InputElementDescription(InputElementSemantic::Bitangent, 0, GraphicsFormat::R32G32B32_Float, 0, 36, 0),
				InputElementDescription(InputElementSemantic::TexCoord,  0, GraphicsFormat::R32G32_Float,    0, 48, 0),
			};
		}
	};
}
