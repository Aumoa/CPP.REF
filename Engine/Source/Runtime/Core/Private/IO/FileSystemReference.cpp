// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/FileSystemReference.h"
#include "IO/DirectoryReference.h"

FileSystemReference::FileSystemReference()
	: _path(TEXT("."))
	, _filepath(".")
{
}

FileSystemReference::FileSystemReference(String path)
	: _path(path)
	, _filepath(path.c_str())
{
}

bool FileSystemReference::IsExists() const noexcept
{
	std::error_code ec;
	return std::filesystem::exists(this->_get_path(), ec);
}

String FileSystemReference::GetPath() const noexcept
{
	return _path;
}

std::optional<DirectoryReference> FileSystemReference::GetParent() const
{
	if (HasParent())
	{
		return DirectoryReference(String(_filepath.parent_path().wstring()));
	}
	else
	{
		return std::nullopt;
	}
}

bool FileSystemReference::HasParent() const noexcept
{
	return _filepath.has_parent_path();
}

String FileSystemReference::GetName() const
{
	return String(_filepath.stem().wstring());
}

FileSystemReference& FileSystemReference::operator =(const FileSystemReference& r)
{
	_path = r._path;
	_filepath = r._filepath;
	return *this;
}

FileSystemReference& FileSystemReference::operator =(FileSystemReference&& rr) noexcept
{
	_path = std::move(rr._path);
	_filepath = std::move(rr._filepath);
	return *this;
}