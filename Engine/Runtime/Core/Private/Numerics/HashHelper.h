// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Object.h"

class HashHelper abstract final : virtual public Object
{
public:
	static size_t GetHashCode(double value);
};