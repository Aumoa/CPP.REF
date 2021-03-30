// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Core/Object.h"

interface IDisposable : virtual public Object
{
	virtual void Dispose() = 0;
};