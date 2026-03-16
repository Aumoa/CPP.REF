// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/VertexFactory.h"
#include "Rendering/InputElementDescription.h"
#include "Rendering/InputElementSemantic.h"
#include "Rendering/GraphicsFormat.h"
#include "PositionColorVertexFactory.gen.h"

namespace Ayla
{
	/// <summary>
	/// Vertex factory for position + color vertices.
	/// Used for debug rendering and simple colored geometry.
	/// </summary>
	ACLASS()
	class RENDERCORE_API PositionColorVertexFactory : public VertexFactory
	{
		GENERATED_BODY()

	public:
		ACONSTRUCTOR()
		PositionColorVertexFactory() = default;

		virtual std::vector<InputElementDescription> GetInputElements() const override
		{
			return
			{
				InputElementDescription(InputElementSemantic::Position, 0, GraphicsFormat::R32G32B32_Float,   0,  0, 0),
				InputElementDescription(InputElementSemantic::Color,    0, GraphicsFormat::R32G32B32A32_Float, 0, 12, 0),
			};
		}
	};
}
