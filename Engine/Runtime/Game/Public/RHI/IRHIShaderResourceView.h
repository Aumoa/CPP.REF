// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface GAME_API IRHIShaderResourceView : virtual public Object
{
	vs_property_get_pure(size_t, Count);
};