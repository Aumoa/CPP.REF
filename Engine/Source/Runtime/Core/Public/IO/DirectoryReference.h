// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FileSystemReference.h"
#include <filesystem>

namespace libty::inline Core
{
	class CORE_API DirectoryReference : public FileSystemReference
	{
	public:
		DirectoryReference() = default;
		DirectoryReference(const DirectoryReference& rhs) = default;
		DirectoryReference(DirectoryReference&& rhs) = default;
		DirectoryReference(const std::filesystem::path& filepath);

		std::filesystem::path GetName() const;
		void CreateIfNotExists(bool bRecursive = false) const;

		DirectoryReference& operator =(const DirectoryReference& rhs) = default;
		DirectoryReference& operator =(DirectoryReference&& rhs) = default;

		auto operator <=>(const DirectoryReference&) const = default;
		bool operator ==(const DirectoryReference&) const = default;
	};
}