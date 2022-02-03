// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "SceneRendering/RenderTargets/SceneRenderTarget.h"

GENERATE_BODY(SSceneRenderTarget);

SSceneRenderTarget::SSceneRenderTarget() : Super()
{
}

void SSceneRenderTarget::Dispose()
{
	Dispose(true);
}