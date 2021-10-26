// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneViewScope.h"
#include "SceneView.h"
#include "SceneRenderTarget.h"

interface IRHIDeviceContext;
class SScene;

class GAME_API SceneRenderer
{
	GENERATED_BODY(SceneRenderer)

public:
	SceneRenderTarget& RenderTarget;

public:
	SceneRenderer(SceneRenderTarget& InRenderTarget);

	void PopulateCommandLists(IRHIDeviceContext* Context);

protected:
	virtual void OnPopulateCommandLists(IRHIDeviceContext* Context) = 0;
};