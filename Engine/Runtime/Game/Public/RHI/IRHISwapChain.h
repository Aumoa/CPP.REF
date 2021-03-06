// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIResource;

interface IRHISwapChain : virtual public Object
{
	virtual void Present() = 0;
	virtual void ResizeBuffers(int32 width, int32 height) = 0;
	virtual IRHIResource* GetBuffer(size_t index) const = 0;

	vs_property_get_pure(size_t, CurrentBackBufferIndex);
	vs_property_get_pure(size_t, NumBuffers);
};