// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "D3D12Minimal.h"

interface ID3D12ResourceBase : virtual public Object
{
	vs_property_get(ID3D12Resource*, Resource);
	virtual ID3D12Resource* Resource_get() const = 0;
};