// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Renderer/SceneRenderer.h"

class SRenderContext;

class SLATECORE_API SSlateRenderer : implements SObject
{
	GENERATED_BODY(SSlateRenderer)

public:
	SSlateRenderer(IRHIDevice* device);

	virtual void BeginFrame(SRenderContext* renderContext);
	virtual void EndFrame();
};