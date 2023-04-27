// Copyright 2020-2022 Aumoa.lib. All right reserved.

import Core.System;

template<>
String TRealType<float_t>::ToString() const
{
	return String(std::to_wstring(Value));
}

template<>
String TRealType<double_t>::ToString() const
{
	return String(std::to_wstring(Value));
}