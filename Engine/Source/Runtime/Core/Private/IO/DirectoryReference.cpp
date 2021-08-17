// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "IO/DirectoryReference.h"

DirectoryReference::DirectoryReference(const std::filesystem::path& filepath) : Super(filepath)
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