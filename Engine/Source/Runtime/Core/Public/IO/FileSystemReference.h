// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreTypes/String.h"
#include <optional>
#include <filesystem>

class DirectoryReference;

class CORE_API FileSystemReference
{
private:
	String _path;
	std::filesystem::path _filepath;

public:
	FileSystemReference();
	FileSystemReference(const FileSystemReference& rhs) = default;
	FileSystemReference(FileSystemReference && rhs) noexcept = default;
	FileSystemReference(String path);

	bool IsExists() const noexcept;
	String GetPath() const noexcept;
	std::optional<DirectoryReference> GetParent() const;
	bool HasParent() const noexcept;
	String GetName() const;

	FileSystemReference& operator =(const FileSystemReference& r);
	FileSystemReference& operator =(FileSystemReference&& rr) noexcept;

	inline auto operator <=>(const FileSystemReference& rhs) const noexcept { return _path <=> rhs._path; }
	inline bool operator ==(const FileSystemReference& rhs) const noexcept { return _path == rhs._path; }

protected:
	inline const std::filesystem::path& _get_path() const noexcept { return _filepath; }
};