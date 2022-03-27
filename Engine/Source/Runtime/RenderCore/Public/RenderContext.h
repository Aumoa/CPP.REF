// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "RHI/RHIInterfaces.h"

class RENDERCORE_API SRenderContext : implements SObject
{
	GENERATED_BODY(SRenderContext)

public:
	SRenderContext();

	SPROPERTY(OwningDevice)
	IRHIDevice* OwningDevice = nullptr;
	SPROPERTY(RenderQueue)
	IRHICommandQueue* RenderQueue = nullptr;
	SPROPERTY(CommandAllocator)
	IRHICommandAllocator* CommandAllocator = nullptr;
	SPROPERTY(CommandList)
	IRHIGraphicsCommandList* CommandList = nullptr;
};