// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Rendering/SceneView.h"

SceneView::SceneView(RenderScene* InScene)
	: Scene(InScene)
{
}

void SceneView::SetupView(const MinimalViewInfo& InView)
{
	CameraView = InView;
}