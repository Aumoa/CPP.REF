// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"

class LogCategory
{
	String _name;

public:
	constexpr LogCategory(const String& name) noexcept
		: _name(name)
	{
	}

	constexpr String GetCategoryName() const noexcept
	{
		return _name;
	}
};