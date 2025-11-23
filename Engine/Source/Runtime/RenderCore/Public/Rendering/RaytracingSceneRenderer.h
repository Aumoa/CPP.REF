// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Rendering/SceneRenderer.h"

namespace Ayla
{
	class RENDERCORE_API RaytracingSceneRenderer : public SceneRenderer
	{
	public:
		RaytracingSceneRenderer();

		virtual void Render(const SceneView& view) override;
	};
}