// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/StaticClass.h"
#include "System/String.h"
#include "IO/SearchOption.h"
#include <vector>

class CORE_API Directory : public StaticClass
{
public:
	static std::vector<String> GetFiles(String InPath, ESearchOption InSearchOption = ESearchOption::TopDirectoryOnly);
	static std::vector<String> GetDirectories(String InPath, ESearchOption InSearchOption = ESearchOption::TopDirectoryOnly);
	static bool Exists(String InPath);
	static bool CreateDirectory(String InPath);
};