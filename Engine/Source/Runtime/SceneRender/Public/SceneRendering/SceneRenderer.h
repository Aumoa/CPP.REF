// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneViewScope.h"
#include "SceneView.h"
#include "SceneRenderTarget.h"

interface IRHIDeviceContext;
class SScene;

class SCENERENDER_API SceneRenderer
{
public:
	SScene* const Scene;
	std::vector<SceneView> Views;

public:
	SceneRenderer(SScene* InScene);

	void InitViews(IRHIDeviceContext* Context, std::span<const SceneViewScope> InViews);
	void PopulateCommandLists(IRHIDeviceContext* Context, const SceneRenderTarget& InRenderTarget);
};