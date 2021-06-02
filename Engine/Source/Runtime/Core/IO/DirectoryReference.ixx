// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:DirectoryReference;

import std.filesystem;
import :FileSystemReference;

using namespace std;

/// <summary>
/// Represents directory reference.
/// </summary>
export class DirectoryReference : public FileSystemReference
{
public:
	using Super = FileSystemReference;

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
	DirectoryReference(const filesystem::path& filepath);

	/// <summary>
	/// Get directory name.
	/// </summary>
	filesystem::path GetName() const;

	/// <summary>
	/// Create directory if not exists.
	/// </summary>
	/// <param name="bRecursive"> Create directory recursively. </param>
	void CreateIfNotExists(bool bRecursive = false) const;

	DirectoryReference& operator =(const DirectoryReference& rhs) = default;
	DirectoryReference& operator =(DirectoryReference&& rhs) = default;
};