// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <filesystem>

namespace libty::inline Core
{
	class File
	{
	public:
		static inline bool Exists(const std::filesystem::path& path) noexcept
		{
			std::error_code ec;
			return std::filesystem::is_regular_file(path, ec);
		}

		static inline bool TryMove(const std::filesystem::path& src, const std::filesystem::path& dst, bool overwrite = false) noexcept
		{
			if (overwrite && Exists(dst))
			{
				if (!TryDelete(dst))
				{
					return false;
				}
			}

			std::error_code ec;
			std::filesystem::rename(src, dst, ec);
			return (bool)ec;
		}

		static inline bool TryDelete(const std::filesystem::path& src) noexcept
		{
			if (Exists(src))
			{
				std::error_code ec;
				return std::filesystem::remove(src, ec);
			}
			return false;
		}
	};
}