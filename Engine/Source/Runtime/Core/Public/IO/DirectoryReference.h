// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "FileSystemReference.h"
#include <filesystem>

/// <summary>
/// Represents directory reference.
/// </summary>
class CORE_API DirectoryReference : public FileSystemReference
{
	CLASS_BODY(DirectoryReference)

public:
	/// <summary>
	/// Initialize new <see cref="DirectoryReference"/> instance.
	/// </summary>
	DirectoryReference() = default;

	/// <summary>
	/// Initialize new <see cref="DirectoryReference"/> instance.
	/// </summary>
	DirectoryReference(const DirectoryReference& rhs) = default;

	/// <summary>
	/// Initialize new <see cref="DirectoryReference"/> instance.
	/// </summary>
	DirectoryReference(DirectoryReference&& rhs) = default;

	/// <summary>
	/// Initialize new <see cref="DirectoryReference"/> instance.
	/// </summary>
	DirectoryReference(const std::filesystem::path& filepath);

	/// <summary>
	/// Get directory name.
	/// </summary>
	std::filesystem::path GetName() const;

	/// <summary>
	/// Create directory if not exists.
	/// </summary>
	/// <param name="bRecursive"> Create directory recursively. </param>
	void CreateIfNotExists(bool bRecursive = false) const;

	DirectoryReference& operator =(const DirectoryReference& rhs) = default;
	DirectoryReference& operator =(DirectoryReference&& rhs) = default;
};