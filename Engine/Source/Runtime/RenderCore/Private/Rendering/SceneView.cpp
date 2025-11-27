// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/SceneView.h"
#include "Rendering/RenderTexture.h"

namespace Ayla
{
	SceneView::SceneView(MinimalViewInfo view)
		: m_View(std::move(view))
	{
	}

	SceneView::~SceneView() noexcept
	{
	}
}