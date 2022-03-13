// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/FileSystemReference.h"
#include "IO/DirectoryReference.h"
#include "Misc/Exceptions.h"

FileSystemReference::FileSystemReference(const std::filesystem::path& filepath)
	: _path(std::filesystem::absolute(filepath))
{
}

bool FileSystemReference::IsExists() const
{
	Xassert(IsSet(), "Path is not setted.");
	return std::filesystem::exists(_path.value_or(L"\\"));
}

std::filesystem::path FileSystemReference::GetPath() const
{
	Xassert(IsSet(), "Path is not setted.");
	return _path.value_or(L"\\");
}

DirectoryReference FileSystemReference::GetParent() const
{
	Xassert(IsSet(), "Path is not setted.");
	return _path.value_or(L"\\").parent_path();
}

bool FileSystemReference::IsSet() const
{
	return _path.has_value();
}

void FileSystemReference::Xassert(bool expression, std::string_view message)
{
	if (!expression)
	{
		throw invalid_operation(message);
	}
}