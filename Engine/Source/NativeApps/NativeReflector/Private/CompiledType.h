// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "Misc/StringBuilder.h"

class CompiledType
{
public:
	CompiledType() noexcept
	{
	}

	virtual ~CompiledType() noexcept
	{
	}

	virtual void Generate(StringBuilder& sb, const String& fileKey) = 0;
};