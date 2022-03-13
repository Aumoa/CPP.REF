// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "GC/SharedPtr.h"

interface IDisposable : implements SObject
{
	GENERATED_INTERFACE_BODY(IDisposable)

	// Special constructor for IDisposable.
	IDisposable()
	{
		this->SObject::bHasFinalizer = true;
	}

	virtual void Dispose() = 0;
};