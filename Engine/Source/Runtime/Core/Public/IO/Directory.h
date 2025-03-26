// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "StaticClass.h"
#include "String_.h"
#include "IO/SearchOption.h"
#include <vector>

namespace Ayla
{
	class CORE_API Directory : public StaticClass
	{
	public:
		static std::vector<String> GetFiles(String InPath, SearchOption InSearchOption = SearchOption::TopDirectoryOnly);
		static std::vector<String> GetDirectories(String InPath, SearchOption InSearchOption = SearchOption::TopDirectoryOnly);
		static bool Exists(String InPath);
		static bool CreateDirectory(String InPath);
		static void Delete(String InPath, bool bRecursive);
	};
}