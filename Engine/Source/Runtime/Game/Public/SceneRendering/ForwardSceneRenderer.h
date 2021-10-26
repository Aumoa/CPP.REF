// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneRenderer.h"

class GAME_API ForwardSceneRenderer : public SceneRenderer
{
	GENERATED_BODY(ForwardSceneRenderer)

public:
	SScene* const Scene;
	std::vector<SceneView> Views;

public:
	ForwardSceneRenderer(SceneRenderTarget& InRenderTarget, SScene* InScene);

	void InitViews(std::span<const SceneViewScope> InViews);

protected:
	virtual void OnPopulateCommandLists(IRHIDeviceContext* Context) override;
};