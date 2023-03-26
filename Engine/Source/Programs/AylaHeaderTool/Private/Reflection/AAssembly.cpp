// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/AAssembly.h"

AAssembly::AAssembly(String InAPIName)
	: APIName(InAPIName)
{
}

String AAssembly::GetAPIName() const
{
	return APIName;
}