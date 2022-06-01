// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include <optional>
#include <filesystem>

class DirectoryReference;

class CORE_API FileSystemReference
{
private:
	String _path;
	mutable std::optional<std::filesystem::path> _fpath;

public:
	inline constexpr FileSystemReference() noexcept
		: _path(TEXT("."))
	{
	}

	inline constexpr FileSystemReference(const FileSystemReference& rhs) noexcept
		: _path(rhs._path)
	{
	}

	inline constexpr FileSystemReference(FileSystemReference&& rhs) noexcept
		: _path(std::move(rhs._path))
	{
	}

	inline constexpr FileSystemReference(String path) noexcept
		: _path(path)
	{
	}

	inline bool IsExists() const
	{
		std::error_code ec;
		return std::filesystem::exists(this->_Get_path(), ec);
	}

	inline constexpr String GetPath() const noexcept
	{
		return _path;
	}

	std::optional<DirectoryReference> GetParent() const;
		
	inline bool HasParent() const
	{
		return this->_Get_path().has_parent_path();
	}

	inline String GetName() const
	{
		return String(this->_Get_path().stem().wstring());
	}

	inline constexpr FileSystemReference& operator =(const FileSystemReference& rhs) noexcept
	{
		_path = rhs._path;
		_fpath = rhs._fpath;
		return *this;
	}

	inline constexpr FileSystemReference& operator =(FileSystemReference&& rhs) noexcept
	{
		_path = std::move(rhs._path);
		_fpath = std::move(rhs._fpath);
		return *this;
	}

	inline constexpr auto operator <=>(const FileSystemReference& rhs) const noexcept
	{
		return _path <=> rhs._path;
	}

	inline constexpr bool operator ==(const FileSystemReference& rhs) const noexcept
	{
		return _path == rhs._path;
	}

protected:
	inline const std::filesystem::path& _Get_path() const noexcept
	{
		if (!_fpath.has_value())
		{
			_fpath.emplace((const wchar_t*)_path);
		}
		return *_fpath;
	}
};