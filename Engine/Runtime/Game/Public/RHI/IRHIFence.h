// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIImmediateCommandList;

interface IRHIFence : virtual public Object
{
	virtual uint64 GetCompletedValue() const = 0;
	virtual void Signal(IRHIImmediateCommandList* queue, uint64 number) = 0;
};