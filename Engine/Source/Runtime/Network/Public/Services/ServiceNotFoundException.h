// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class ServiceNotFoundException : public Exception
{
public:
	ServiceNotFoundException(const std::type_info& ti)
		: Exception(TEXTF("Cannot find service from service collection.", String(ti.name())))
	{
	}
};