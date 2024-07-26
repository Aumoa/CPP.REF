// Copyright 2020-2022 Aumoa.lib. All right reserved.

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