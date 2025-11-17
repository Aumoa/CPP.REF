// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class SceneView;

	class RENDERCORE_API SceneRenderer
	{
	protected:
		SceneRenderer();

	public:
		virtual ~SceneRenderer() noexcept;

		virtual void Render(const SceneView& view) = 0;
	};
}