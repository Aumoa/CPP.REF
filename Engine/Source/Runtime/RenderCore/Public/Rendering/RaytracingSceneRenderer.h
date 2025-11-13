// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Rendering/SceneRenderer.h"
#include "RaytracingSceneRenderer.gen.h"

namespace Ayla
{
	ACLASS()
	class RENDERCORE_API RaytracingSceneRenderer : public SceneRenderer
	{
		GENERATED_BODY()

	public:
		ACONSTRUCTOR()
		RaytracingSceneRenderer();

		virtual void Render_Implementation(SharedPtr<SceneView> view) override;
	};
}