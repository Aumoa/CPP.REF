// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:FileReference;

import std.core;
import std.filesystem;
import :FileSystemReference;

using namespace std;

export class Object;

/// <summary>
/// Represents file reference.
/// </summary>
export class FileReference : public FileSystemReference
{
public:
	using Super = FileSystemReference;

private:
	wfstream _sharedstream;
	set<const Object*> _shared;

public:
	/// <summary>
	/// Initialize new <see cref="FileReference"/> instance.
	/// </summary>
	FileReference() = default;

	/// <summary>
	/// Initialize new <see cref="FileReference"/> instance.
	/// </summary>
	FileReference(const FileReference& rhs) = default;

	/// <summary>
	/// Initialize new <see cref="FileReference"/> instance.
	/// </summary>
	FileReference(FileReference&& rhs) = default;

	/// <summary>
	/// Initialize new <see cref="FileReference"/> instance.
	/// </summary>
	FileReference(const filesystem::path& filepath);
	~FileReference();

	/// <summary>
	/// Get file name with extensions.
	/// </summary>
	filesystem::path GetFilename() const;

	/// <summary>
	/// Get file name without extensions.
	/// </summary>
	filesystem::path GetName() const;

	/// <summary>
	/// Get extension.
	/// </summary>
	filesystem::path GetExtension() const;

	/// <summary>
	/// Open file stream.
	/// </summary>
	/// <param name="mode"> The opening mode. </param>
	/// <param name="bCreateIfNotExists"> Always create new file if not exists. </param>
	/// <param name="bCreateDirectoryRecursive"> Create parent directory recursively. </param>
	wfstream OpenStream(ios_base::openmode mode = ios_base::in | ios_base::out, bool bCreateIfNotExists = true, bool bCreateDirectoryRecursive = true) const;

	/// <summary>
	/// Open shared file stream. If file already open as stream, get it.
	/// </summary>
	/// <param name="sharingUser"> Key for shared user list. </param>
	/// <param name="mode"> The opening mode. </param>
	/// <param name="bCreateIfNotExists"> Always create new file if not exists. </param>
	/// <param name="bCreateDirectoryRecursive"> Create parent directory recursively. </param>
	wfstream& OpenSharedStream(const Object* sharingUser, ios_base::openmode mode = ios_base::in | ios_base::out, bool bCreateIfNotExists = true, bool bCreateDirectoryRecursive = true);

	/// <summary>
	/// Close shared file stream.
	/// </summary>
	/// <param name="sharingUser"> Key for shared user list. </param>
	void CloseSharedStream(const Object* sharingUser);

	FileReference& operator =(const FileReference& rhs) = default;
	FileReference& operator =(FileReference&& rhs) = default;

private:
	void FlushAndCloseSharedStream();
};