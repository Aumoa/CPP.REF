// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

class CORE_API ConstructorNotFoundException : public Exception
{
public:
	ConstructorNotFoundException(String className);
};