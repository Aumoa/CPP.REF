// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "FileSystemReference.h"
#include <filesystem>

/// <summary>
/// Represents directory reference.
/// </summary>
class CORE_API SDirectoryReference : public SFileSystemReference
{
	GENERATED_BODY(SDirectoryReference)

public:
	/// <summary>
	/// Initialize new <see cref="SDirectoryReference"/> instance.
	/// </summary>
	SDirectoryReference() = default;

	/// <summary>
	/// Initialize new <see cref="SDirectoryReference"/> instance.
	/// </summary>
	SDirectoryReference(const SDirectoryReference& rhs) = default;

	/// <summary>
	/// Initialize new <see cref="SDirectoryReference"/> instance.
	/// </summary>
	SDirectoryReference(SDirectoryReference&& rhs) = default;

	/// <summary>
	/// Initialize new <see cref="SDirectoryReference"/> instance.
	/// </summary>
	SDirectoryReference(const std::filesystem::path& filepath);

	/// <summary>
	/// Get directory name.
	/// </summary>
	std::filesystem::path GetName() const;

	/// <summary>
	/// Create directory if not exists.
	/// </summary>
	/// <param name="bRecursive"> Create directory recursively. </param>
	void CreateIfNotExists(bool bRecursive = false) const;

	SDirectoryReference& operator =(const SDirectoryReference& rhs) = default;
	SDirectoryReference& operator =(SDirectoryReference&& rhs) = default;
};