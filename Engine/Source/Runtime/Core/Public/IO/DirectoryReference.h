// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FileSystemReference.h"

namespace libty::inline Core
{
	class DirectoryReference : public FileSystemReference
	{
	public:
		inline constexpr DirectoryReference() noexcept
		{
		}

		inline constexpr DirectoryReference(const DirectoryReference& rhs) noexcept
			: FileSystemReference(rhs)
		{
		}

		inline constexpr DirectoryReference(DirectoryReference&& rhs) noexcept
			: FileSystemReference(std::move(rhs))
		{
		}

		inline constexpr DirectoryReference(String path) noexcept
			: FileSystemReference(path)
		{
		}

		inline bool CreateIfNotExists(bool bRecursive = false) const noexcept
		{
			if (!IsExists())
			{
				if (bRecursive)
				{
					std::error_code ec;
					return std::filesystem::create_directories(this->_Get_path(), ec);
				}
				else
				{
					std::error_code ec;
					return std::filesystem::create_directory(this->_Get_path(), ec);
				}
			}

			return true;
		}
	};
}