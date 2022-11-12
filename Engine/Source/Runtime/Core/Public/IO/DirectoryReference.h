// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FileSystemReference.h"

class FileReference;

class CORE_API DirectoryReference : public FileSystemReference
{
public:
	DirectoryReference() = default;
	DirectoryReference(const DirectoryReference& rhs) = default;
	DirectoryReference(DirectoryReference&& rhs) noexcept = default;
	DirectoryReference(String path);

	bool CreateIfNotExists(bool bRecursive = false) const;
	std::vector<FileReference> GetAllFiles(bool recursive = false) const;
	FileReference GetFile(const String& filename) const;
};