// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <optional>
#include <filesystem>

class DirectoryReference;

/// <summary>
/// Represents file system reference.
/// </summary>
class FileSystemReference
{
	std::optional<std::filesystem::path> _path;

public:
	/// <summary>
	/// Initialize new <see cref="FileSystemReference"/> instance.
	/// </summary>
	FileSystemReference() = default;

	/// <summary>
	/// Initialize new <see cref="FileSystemReference"/> instance.
	/// </summary>
	FileSystemReference(const FileSystemReference& rhs) = default;

	/// <summary>
	/// Initialize new <see cref="FileSystemReference"/> instance.
	/// </summary>
	FileSystemReference(FileSystemReference&& rhs) = default;

	/// <summary>
	/// Initialize new <see cref="FileSystemReference"/> instance.
	/// </summary>
	FileSystemReference(const std::filesystem::path& filepath);

	/// <summary>
	/// Represents the file is exists.
	/// </summary>
	bool IsExists() const;

	/// <summary>
	/// Get path.
	/// </summary>
	std::filesystem::path GetPath() const;

	/// <summary>
	/// Get parent reference.
	/// </summary>
	DirectoryReference GetParent() const;

	/// <summary>
	/// Represents file refernce path is setted.
	/// </summary>
	bool IsSet() const;

	FileSystemReference& operator =(const FileSystemReference& rhs) = default;
	FileSystemReference& operator =(FileSystemReference&& rhs) = default;
};