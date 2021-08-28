// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "IO/FileSystemReference.h"
#include "IO/DirectoryReference.h"

FileSystemReference::FileSystemReference(const std::filesystem::path& filepath) : Super()
	, _path(std::filesystem::absolute(filepath))
{
}

bool FileSystemReference::IsExists() const
{
	return std::filesystem::exists(_path.value_or(L"\\"));
}

std::filesystem::path FileSystemReference::GetPath() const
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