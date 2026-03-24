// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/VertexFactory.h"
#include "Rendering/InputElementDescription.h"
#include "Rendering/InputElementSemantic.h"
#include "Rendering/GraphicsFormat.h"

namespace Ayla
{
	/// <summary>
	/// Vertex factory for PrimitiveVertex (Position + Normal + TexCoord).
	/// This is the default vertex format used for standard mesh rendering.
	/// </summary>
	class RENDERCORE_API PrimitiveVertexFactory : public VertexFactory
	{
	public:
		PrimitiveVertexFactory() = default;

		virtual std::vector<InputElementDescription> GetInputElements() const override
		{
			return
			{
				InputElementDescription(InputElementSemantic::Position, 0, GraphicsFormat::R32G32B32_Float, 0,  0,  0),
				InputElementDescription(InputElementSemantic::Normal,   0, GraphicsFormat::R32G32B32_Float, 0, 12,  0),
				InputElementDescription(InputElementSemantic::TexCoord, 0, GraphicsFormat::R32G32_Float,    0, 24,  0),
			};
		}
	};
}
