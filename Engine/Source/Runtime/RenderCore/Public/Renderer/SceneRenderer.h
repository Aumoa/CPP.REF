// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "IDisposable.h"
#include "RHI/RHIInterfaces.h"

class RENDERCORE_API SSceneRenderer : implements SObject
{
	GENERATED_BODY(SSceneRenderer)

private:
	SPROPERTY(_device)
	IRHIDevice* _device = nullptr;

	SPROPERTY(_commandAllocator)
	IRHICommandAllocator* _commandAllocator = nullptr;
	SPROPERTY(_commandBuffer)
	IRHIGraphicsCommandList* _commandBuffer = nullptr;

public:
	SSceneRenderer(IRHIDevice* device);

	virtual void BeginDraw();
	virtual void EndDraw();
};