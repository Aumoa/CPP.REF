// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "RHI/RHIInterfaces.h"

class SRenderContext;

class RENDERCORE_API SRenderEngine : implements SObject
{
	GENERATED_BODY(SRenderEngine)

private:
	SPROPERTY(_device)
	IRHIDevice* _device = nullptr;
	SPROPERTY(_commandQueue)
	IRHICommandQueue* _commandQueue = nullptr;

public:
	SRenderEngine();

	virtual SRenderContext* CreateRenderContext();
};