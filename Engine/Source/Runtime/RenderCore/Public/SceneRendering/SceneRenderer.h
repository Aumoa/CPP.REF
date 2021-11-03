// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIEnums.h"

class SceneRenderContext;

class RENDERCORE_API SceneRenderer
{
	GENERATED_BODY(SceneRenderer)

private:
	ERHIResourceStates InitialState;
	bool bRestoreTransition;
		
public:
	SceneRenderContext* const RenderContext = nullptr;

public:
	SceneRenderer(SceneRenderContext* RenderContext, ERHIResourceStates InitialState, bool bRestoreTransition = false);

	virtual void BeginDraw();
	virtual void EndDraw();
};