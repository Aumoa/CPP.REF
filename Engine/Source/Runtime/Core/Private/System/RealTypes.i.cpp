// Copyright 2020-2024 Aumoa.lib. All right reserved.

import Core;

#include "System/LanguageSupportMacros.h"

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