// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/Directory.h"
#include "CoreTypes/String.h"
#include "Exceptions/SystemException.h"
#include <filesystem>

bool Directory::TryCreateDirectory(const String& path, std::error_code* ec)
{
	std::error_code ec_;
	bool success = std::filesystem::create_directories((std::wstring)path, ec_);
	if (ec)
	{
		if (success)
		{
			ec->clear();
		}
		else
		{
			*ec = ec_;
		}
	}
	return success;
}

void Directory::CreateDirectory(const String& path)
{
	std::error_code ec_;
	if (std::filesystem::create_directories((std::wstring)path, ec_) == false)
	{
		throw SystemException(ec_);
	}
}

bool Directory::Exists(const String& path)
{
	std::error_code ec;
	return std::filesystem::is_directory((std::wstring)path, ec);
}