// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IDisposable.h"

using namespace libty;

IDisposable::IDisposable()
{
	SObject::bHasFinalizer = true;
}