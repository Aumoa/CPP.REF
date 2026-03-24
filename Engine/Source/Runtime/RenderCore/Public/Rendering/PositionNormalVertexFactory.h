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
	/// Vertex factory for position + normal vertices.
	/// Used for simple diffuse lighting without texture mapping.
	/// </summary>
	class RENDERCORE_API PositionNormalVertexFactory : public VertexFactory
	{
	public:
		PositionNormalVertexFactory() = default;

		virtual std::vector<InputElementDescription> GetInputElements() const override
		{
			return
			{
				InputElementDescription(InputElementSemantic::Position, 0, GraphicsFormat::R32G32B32_Float, 0,  0, 0),
				InputElementDescription(InputElementSemantic::Normal,   0, GraphicsFormat::R32G32B32_Float, 0, 12, 0),
			};
		}
	};
}
