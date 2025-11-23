// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class RenderTexture;

	class RENDERCORE_API SceneView
	{
	private:
		const SharedPtr<RenderTexture> m_OutputTexture;

	public:
		SceneView() = delete;
		SceneView(SharedPtr<RenderTexture> outputTexture);
		SceneView(const SceneView&) = delete;
		~SceneView() noexcept;
	};
}