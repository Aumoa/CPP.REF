// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIFactory;

interface IRHIFactoryChild : implements SObject, implements IDisposable
{
	GENERATED_INTERFACE_BODY(IRHIFactoryChild)

	virtual IRHIFactory* GetFactory() = 0;
};