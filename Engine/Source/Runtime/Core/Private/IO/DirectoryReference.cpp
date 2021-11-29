// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "IO/DirectoryReference.h"

SDirectoryReference::SDirectoryReference(const std::filesystem::path& filepath) : Super(filepath)
{
}

std::filesystem::path SDirectoryReference::GetName() const
{
	return GetPath().stem();
}

void SDirectoryReference::CreateIfNotExists(bool bRecursive) const
{
	if (IsExists())
	{
		return;
	}

	if (bRecursive)
	{
		SDirectoryReference dr = GetParent();
		dr.CreateIfNotExists(bRecursive);
	}

	std::filesystem::create_directory(GetPath() / std::filesystem::path());
}