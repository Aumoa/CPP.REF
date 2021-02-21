// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface GAME_API IRHIUnorderedAccessView : virtual public Object
{
	vs_property_get(size_t, Count);
	virtual size_t Count_get() const = 0;
};