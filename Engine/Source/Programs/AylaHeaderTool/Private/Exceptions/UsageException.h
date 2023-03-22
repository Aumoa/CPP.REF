// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class UsageException : public Exception
{
	String ParameterName;

public:
	UsageException(String InParamterName);

	String GetParameterName() const;
};