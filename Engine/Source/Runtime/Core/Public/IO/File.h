// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <filesystem>

class File
{
public:
	static inline bool Exists(const String& path) noexcept
	{
		std::error_code ec;
		return std::filesystem::is_regular_file((std::wstring)path, ec);
	}

	static inline bool TryMove(const String& src, const String& dst, bool overwrite = false) noexcept
	{
		if (overwrite && Exists(dst))
		{
			if (!TryDelete(dst)
			{
				return false;
			}
		}

		std::error_code ec;
		std::filesystem::rename((std::wstring)src, (std::wstring)dst, ec);
		return (bool)ec;
	}

	static inline bool TryDelete(const String& src) noexcept
	{
		if (Exists(src))
		{
			std::error_code ec;
			return std::filesystem::remove((std::wstring)src, ec);
		}
		return false;
	}
};