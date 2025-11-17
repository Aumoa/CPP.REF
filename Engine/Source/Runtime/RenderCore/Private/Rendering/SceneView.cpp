// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/SceneView.h"
#include "Rendering/RenderTexture.h"

namespace Ayla
{
	SceneView::SceneView(SharedPtr<RenderTexture> outputTexture)
		: m_OutputTexture(outputTexture)
	{
	}

	SceneView::~SceneView() noexcept
	{
	}
}