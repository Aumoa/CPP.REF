// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "D3D12Minimal.h"

interface ID3D12ResourceBase : virtual public Object
{
	vs_property_get_pure(ID3D12Resource*, Resource);
};