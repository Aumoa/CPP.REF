// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"
#include "RenderTexture.gen.h"

namespace Ayla
{
	class CommandBuffer;
	class SceneView;

	ACLASS()
	class RENDERCORE_API RenderTexture : public Object
	{
		GENERATED_BODY()

	public:
		virtual Vector2N GetSize() const = 0;

		virtual void Acquire(CommandBuffer* cmd) = 0;

		// Render using raytracing (backend-specific implementation)
		virtual void RenderRaytracing(CommandBuffer* cmd, const SceneView& view) = 0;
	};
}