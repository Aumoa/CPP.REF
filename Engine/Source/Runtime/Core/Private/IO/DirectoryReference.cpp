// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/DirectoryReference.h"

using namespace libty;

DirectoryReference::DirectoryReference(const std::filesystem::path& filepath)
	: FileSystemReference(filepath)
{
}

std::filesystem::path DirectoryReference::GetName() const
{
	return GetPath().stem();
}

void DirectoryReference::CreateIfNotExists(bool bRecursive) const
{
	if (IsExists())
	{
		return;
	}

	if (bRecursive)
	{
		DirectoryReference dr = GetParent();
		dr.CreateIfNotExists(bRecursive);
	}

	std::filesystem::create_directory(GetPath() / std::filesystem::path());
}