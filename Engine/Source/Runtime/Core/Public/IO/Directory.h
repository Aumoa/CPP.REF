// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <system_error>
#include <vector>
#include "IO/DirectorySearchOptions.h"

class String;

class CORE_API Directory
{
public:
	static bool TryCreateDirectory(String InPath, std::error_code* OutErrorCode);
	static void CreateDirectory(String InPath);
	static bool Exists(String InPath);
	static std::vector<String> GetFiles(String InPath, EDirectorySearchOptions Options);
};