// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/FileSystemReference.h"
#include "IO/DirectoryReference.h"

GENERATE_BODY(SFileSystemReference);

SFileSystemReference::SFileSystemReference(const std::filesystem::path& filepath) : Super()
	, _path(std::filesystem::absolute(filepath))
{
}

bool SFileSystemReference::IsExists() const
{
	return std::filesystem::exists(_path.value_or(L"\\"));
}

std::filesystem::path SFileSystemReference::GetPath() const
{
	return _path.value_or(L"\\");
}

SDirectoryReference SFileSystemReference::GetParent() const
{
	return _path.value_or(L"\\").parent_path();
}

bool SFileSystemReference::IsSet() const
{
	return _path.has_value();
}