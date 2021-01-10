// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIResource : virtual public Object
{
	virtual uint64 GetVirtualAddress() const = 0;
	virtual void* GetMappingAddress() const = 0;
};