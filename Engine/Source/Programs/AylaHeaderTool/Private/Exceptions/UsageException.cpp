// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/UsageException.h"

UsageException::UsageException(String InParameterName)
	: ParameterName(InParameterName)
{
}

String UsageException::GetParameterName() const
{
	return ParameterName;
}