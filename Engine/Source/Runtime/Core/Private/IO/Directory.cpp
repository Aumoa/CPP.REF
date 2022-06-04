// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/Directory.h"
#include "Misc/String.h"
#include <filesystem>

bool Directory::TryCreateDirectory(const String& path)
{
	std::error_code ec;
	return std::filesystem::create_directories((std::wstring)path, ec);
}

bool Directory::Exists(const String& path)
{
	std::error_code ec;
	return std::filesystem::is_directory((std::wstring)path, ec);
}