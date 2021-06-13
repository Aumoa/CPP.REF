// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.filesystem;
import SC.Runtime.Core;

using namespace std;

FileSystemReference::FileSystemReference(const filesystem::path& filepath) : _path(filesystem::absolute(filepath))
{
}

bool FileSystemReference::IsExists() const
{
	return filesystem::exists(_path.value_or(L"\\"));
}

filesystem::path FileSystemReference::GetPath() const
{
	return _path.value_or(L"\\");
}

DirectoryReference FileSystemReference::GetParent() const
{
	return _path.value_or(L"\\").parent_path();
}

bool FileSystemReference::IsSet() const
{
	return _path.has_value();
}