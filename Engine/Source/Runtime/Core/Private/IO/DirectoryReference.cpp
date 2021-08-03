// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "IO/DirectoryReference.h"

using namespace std;

DirectoryReference::DirectoryReference(const filesystem::path& filepath) : Super(filepath)
{
}

filesystem::path DirectoryReference::GetName() const
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

	filesystem::create_directory(GetPath() / filesystem::path());
}