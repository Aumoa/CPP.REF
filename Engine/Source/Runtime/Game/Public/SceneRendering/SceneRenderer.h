// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneRendering/SceneRenderContext.h"

class SScene;

class SSceneRenderer : implements SObject
{
	GENERATED_BODY(SSceneRenderer)

private:
	std::optional<SceneRenderContext> CurrentRenderContext;

public:
	SSceneRenderer();

	virtual void BeginDraw(const SceneRenderContext& RenderContext);
	virtual void EndDraw();
	virtual void DrawScene(SScene* Scene);

protected:
	const SceneRenderContext& GetCurrentRenderContext();
};