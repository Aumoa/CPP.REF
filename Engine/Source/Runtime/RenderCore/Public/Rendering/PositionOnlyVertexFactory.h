// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/VertexFactory.h"
#include "Rendering/InputElementDescription.h"
#include "Rendering/InputElementSemantic.h"
#include "Rendering/GraphicsFormat.h"
#include "PositionOnlyVertexFactory.gen.h"

namespace Ayla
{
	/// <summary>
	/// Vertex factory for position-only vertices.
	/// Used for shadow map rendering and depth prepass.
	/// </summary>
	ACLASS()
	class RENDERCORE_API PositionOnlyVertexFactory : public VertexFactory
	{
		GENERATED_BODY()

	public:
		ACONSTRUCTOR()
		PositionOnlyVertexFactory() = default;

		virtual std::vector<InputElementDescription> GetInputElements() const override
		{
			return
			{
				InputElementDescription(InputElementSemantic::Position, 0, GraphicsFormat::R32G32B32_Float, 0, 0, 0),
			};
		}
	};
}
