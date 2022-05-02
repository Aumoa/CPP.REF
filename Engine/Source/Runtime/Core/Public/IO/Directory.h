// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <filesystem>
#include <stack>

namespace libty::inline Core
{
	class Directory
	{
	public:
		static inline bool TryCreateDirectory(const std::filesystem::path& path)
		{
			std::error_code ec;
			return std::filesystem::create_directories(path, ec);
		}

		static inline bool Exists(std::filesystem::path path)
		{
			std::error_code ec;
			return std::filesystem::is_directory(path, ec);
		}
	};
}