// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Core/Object.h"

class HashHelper abstract final : virtual public Object
{
public:
	static size_t GetHashCode(float value);
};