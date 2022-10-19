// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SceneView;
class SceneRenderer;

struct SceneRenderContext
{
	std::span<SceneView* const> SceneViews;
	SceneRenderer* Renderer;
};