// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "System/RealTypes.h"
#include "System/String.h"
#include "Platform/PlatformLocalization.h"
#include <cstdlib>

template<>
String TRealType<float>::ToString() const
{
	return String::Format(TEXT("{}"), Value);
}

template<>
String TRealType<double>::ToString() const
{
	return String::Format(TEXT("{}"), Value);
}