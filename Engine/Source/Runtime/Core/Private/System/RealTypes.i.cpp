// Copyright 2020-2024 Aumoa.lib. All right reserved.

#include "System/LanguageSupportMacros.h"

import Core;

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