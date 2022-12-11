// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "SystemException.h"

class CORE_API AccessViolationException : public Exception
{
public:
	enum class EAccessMode
	{
		Read,
		Write
	};

public:
	AccessViolationException(EAccessMode access, int64 location);

private:
	static String FormatMessage(EAccessMode access, int64 location);
};