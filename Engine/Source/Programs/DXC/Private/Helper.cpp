// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Helper.h"
#include <iostream>

namespace Ayla
{
	String ReadLine()
	{
		std::wstring s;
		std::getline(std::wcin, s);
		return String(s);
	}
}