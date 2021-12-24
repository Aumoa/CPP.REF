// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "GC/SharedPtr.h"

interface IDisposable : implements SObject
{
	GENERATED_INTERFACE_BODY(IDisposable)

	virtual void Dispose() = 0;
};