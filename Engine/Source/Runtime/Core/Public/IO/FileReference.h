// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "FileSystemReference.h"
#include <set>
#include <fstream>
#include <filesystem>

class SObject;

/// <summary>
/// Represents file reference.
/// </summary>
class CORE_API SFileReference : public SFileSystemReference
{
	GENERATED_BODY(SFileReference)

private:
	std::wfstream _sharedstream;
	std::set<const SObject*> _shared;

public:
	/// <summary>
	/// Initialize new <see cref="SFileReference"/> instance.
	/// </summary>
	SFileReference() = default;

	/// <summary>
	/// Initialize new <see cref="SFileReference"/> instance.
	/// </summary>
	SFileReference(const SFileReference& rhs) = default;

	/// <summary>
	/// Initialize new <see cref="SFileReference"/> instance.
	/// </summary>
	SFileReference(SFileReference&& rhs) = default;

	/// <summary>
	/// Initialize new <see cref="SFileReference"/> instance.
	/// </summary>
	SFileReference(const std::filesystem::path& filepath);
	~SFileReference();

	/// <summary>
	/// Get file name with extensions.
	/// </summary>
	std::filesystem::path GetFilename() const;

	/// <summary>
	/// Get file name without extensions.
	/// </summary>
	std::filesystem::path GetName() const;

	/// <summary>
	/// Get extension.
	/// </summary>
	std::filesystem::path GetExtension() const;

	/// <summary>
	/// Open file stream.
	/// </summary>
	/// <param name="mode"> The opening mode. </param>
	/// <param name="bCreateIfNotExists"> Always create new file if not exists. </param>
	/// <param name="bCreateDirectoryRecursive"> Create parent directory recursively. </param>
	std::wfstream OpenStream(std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out, bool bCreateIfNotExists = true, bool bCreateDirectoryRecursive = true) const;

	/// <summary>
	/// Open shared file stream. If file already open as stream, get it.
	/// </summary>
	/// <param name="sharingUser"> Key for shared user list. </param>
	/// <param name="mode"> The opening mode. </param>
	/// <param name="bCreateIfNotExists"> Always create new file if not exists. </param>
	/// <param name="bCreateDirectoryRecursive"> Create parent directory recursively. </param>
	std::wfstream& OpenSharedStream(SObject* sharingUser, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out, bool bCreateIfNotExists = true, bool bCreateDirectoryRecursive = true);

	/// <summary>
	/// Close shared file stream.
	/// </summary>
	/// <param name="sharingUser"> Key for shared user list. </param>
	void CloseSharedStream(SObject* sharingUser);

	std::string ReadAllText();
	bool WriteAllText(std::string_view Text);

	SFileReference& operator =(const SFileReference& rhs) = default;
	SFileReference& operator =(SFileReference&& rhs) = default;

private:
	void FlushAndCloseSharedStream();
};