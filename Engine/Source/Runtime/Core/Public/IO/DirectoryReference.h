// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FileSystemReference.h"

class FileReference;

class CORE_API DirectoryReference : public FileSystemReference
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

	bool CreateIfNotExists(bool bRecursive = false) const noexcept;
	std::vector<FileReference> GetAllFiles(bool recursive = false) const noexcept;
	FileReference GetFile(const String& filename) const noexcept;
};