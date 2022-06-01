// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

class CORE_API AssertException : public Exception
{
public:
	AssertException(const String& exp);
	AssertException(const String& exp, const String& msg);
};