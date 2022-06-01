// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

class CORE_API ArgumentNullException : public Exception
{
public:
	ArgumentNullException(const String& argName);
};