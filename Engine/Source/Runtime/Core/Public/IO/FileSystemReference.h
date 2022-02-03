// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <optional>
#include <filesystem>
#include "Object.h"

class SDirectoryReference;

/// <summary>
/// Represents file system reference.
/// </summary>
class CORE_API SFileSystemReference : implements SObject
{
	GENERATED_BODY(SFileSystemReference)

private:
	std::optional<std::filesystem::path> _path;

public:
	/// <summary>
	/// Initialize new <see cref="SFileSystemReference"/> instance.
	/// </summary>
	SFileSystemReference() = default;

	/// <summary>
	/// Initialize new <see cref="SFileSystemReference"/> instance.
	/// </summary>
	SFileSystemReference(const SFileSystemReference& rhs) = default;

	/// <summary>
	/// Initialize new <see cref="SFileSystemReference"/> instance.
	/// </summary>
	SFileSystemReference(SFileSystemReference&& rhs) = default;

	/// <summary>
	/// Initialize new <see cref="SFileSystemReference"/> instance.
	/// </summary>
	SFileSystemReference(const std::filesystem::path& filepath);

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
	SDirectoryReference GetParent() const;

	/// <summary>
	/// Represents file refernce path is setted.
	/// </summary>
	bool IsSet() const;

	SFileSystemReference& operator =(const SFileSystemReference& rhs) = default;
	SFileSystemReference& operator =(SFileSystemReference&& rhs) = default;
};