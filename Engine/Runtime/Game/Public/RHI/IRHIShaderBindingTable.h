// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface GAME_API IRHIShaderBindingTable : virtual public Object
{
	vs_property_get(uint64, RayGenerationStartAddress);
	virtual uint64 RayGenerationStartAddress_get() const = 0;
	vs_property_get(uint64, RayGenerationBufferStride);
	virtual uint64 RayGenerationBufferStride_get() const = 0;
};