// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIInterfaces.h"

class SRenderContext;
class SViewport;

class SLATECORE_API SSlateRenderer : implements SObject
{
	GENERATED_BODY(SSlateRenderer)

public:
	SSlateRenderer(IRHIDevice* device);

	virtual void BeginFrame(SRenderContext* renderContext);
	virtual void RenderViewport(SViewport* viewport);
	virtual void EndFrame();
};