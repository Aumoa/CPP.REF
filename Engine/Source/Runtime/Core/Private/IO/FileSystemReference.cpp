// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/FileSystemReference.h"
#include "IO/DirectoryReference.h"
#include "Exceptions/InvalidOperationException.h"
#include "CoreAssert.h"

using namespace libty;

FileSystemReference::FileSystemReference(const std::filesystem::path& filepath)
	: _path(std::filesystem::absolute(filepath))
{
}

bool FileSystemReference::IsExists() const
{
	checkf(IsSet(), TEXT("Path is not setted."));
	return std::filesystem::exists(_path.value_or(L"\\"));
}

std::filesystem::path FileSystemReference::GetPath() const
{
	checkf(IsSet(), TEXT("Path is not setted."));
	return _path.value_or(L"\\");
}

DirectoryReference FileSystemReference::GetParent() const
{
	checkf(IsSet(), TEXT("Path is not setted."));
	return _path.value_or(L"\\").parent_path();
}

bool FileSystemReference::IsSet() const
{
	return _path.has_value();
}