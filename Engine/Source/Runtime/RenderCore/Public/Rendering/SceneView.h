// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/MinimalViewInfo.h"

namespace Ayla
{
	class RenderTexture;

	class RENDERCORE_API SceneView
	{
	private:
		const MinimalViewInfo m_View;

	public:
		SceneView() = delete;
		SceneView(MinimalViewInfo view);
		SceneView(const SceneView&) = delete;
		~SceneView() noexcept;

		const MinimalViewInfo& GetViewInfo() const noexcept { return m_View; }
	};
}