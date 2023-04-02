// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/AAssembly.h"

AAssembly::AAssembly(String InAssemblyName)
	: Name(InAssemblyName)
	, APIName(String::Format(TEXT("{}_API"), InAssemblyName.ToUpper()))
{
}

String AAssembly::GetName() const
{
	return Name;
}

String AAssembly::GetAPIName() const
{
	return APIName;
}