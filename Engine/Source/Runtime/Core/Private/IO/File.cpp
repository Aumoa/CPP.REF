// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/File.h"

bool File::Exists(String path) noexcept
{
	std::error_code ec;
	return std::filesystem::is_regular_file((std::wstring)path, ec);
}

bool File::TryMove(String src, String dst, bool overwrite) noexcept
{
	if (overwrite && Exists(dst))
	{
		if (!TryDelete(dst))
		{
			return false;
		}
	}

	std::error_code ec;
	std::filesystem::rename((std::wstring)src, (std::wstring)dst, ec);
	return (bool)ec;
}

bool File::TryDelete(String src) noexcept
{
	if (Exists(src))
	{
		std::error_code ec;
		return std::filesystem::remove((std::wstring)src, ec);
	}
	return false;
}