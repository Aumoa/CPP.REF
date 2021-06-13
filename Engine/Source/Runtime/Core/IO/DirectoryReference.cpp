// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.filesystem;
import SC.Runtime.Core;

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