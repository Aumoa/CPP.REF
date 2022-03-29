// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct IDisposable : virtual public SObject
{
	GENERATED_BODY(IDisposable);

	// Special constructor for IDisposable.
	IDisposable()
	{
		this->SObject::bHasFinalizer = true;
	}

	virtual void Dispose() = 0;
	virtual bool IsDisposed() = 0;
};