// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "IDisposable.generated.h"

SINTERFACE()
interface CORE_API IDisposable : virtual public Object
{
	GENERATED_BODY()

	SFUNCTION()
	virtual void Dispose() = 0;
};