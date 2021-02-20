﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IRHICommandList.h"

interface IRHIDeferredCommandList;

interface IRHIImmediateCommandList : virtual public Object, virtual public IRHICommandList
{
	virtual void ExecuteCommandList(IRHIDeferredCommandList* deferredCommandList) = 0;
	virtual void Flush() = 0;
};