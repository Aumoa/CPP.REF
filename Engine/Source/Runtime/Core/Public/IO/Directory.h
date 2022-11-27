// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <system_error>

class String;

class CORE_API Directory
{
public:
	static bool TryCreateDirectory(const String& path, std::error_code* ec);
	static void CreateDirectory(const String& path);
	static bool Exists(const String& path);
};