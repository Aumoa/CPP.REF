// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "IO/FileSystemReference.h"
#include "IO/File.h"
#include "System/Path.h"
#include "System/Environment.h"
#include "System/InvalidOperationException.h"

class DirectoryReference;

class CORE_API FileReference : public FileSystemReference
{
public:
	FileReference() noexcept
	{
	}

	FileReference(String InPath) : FileSystemReference(InPath)
	{
	}

	FileReference(const FileReference&) = default;
	FileReference(FileReference&&) noexcept = default;

	virtual bool IsExists() const override
	{
		return File::Exists(GetValue());
	}

	[[nodiscard]] FileReference GetAbsolute() const
	{
		if (IsPathFullQualified())
		{
			return FileReference(GetValue());
		}
		else
		{
			return FileReference(GetAbsolutePath());
		}
	}

	[[nodiscard]] FileReference ToCurrentDirectoryBased() const
	{
		if (IsPathFullQualified())
		{
			throw InvalidOperationException();
		}

		return Path::Combine(Environment::GetCurrentDirectory(), GetValue());
	}

	void Delete() const
	{
		if (IsExists())
		{
			File::Delete(GetValue());
		}
	}

	[[nodiscard]] String ReadAllText() const
	{
		return File::ReadAllText(GetValue());
	}

	[[nodiscard]] Task<String> ReadAllTextAsync(CancellationToken InCancellationToken = {}) const
	{
		return File::ReadAllTextAsync(GetValue(), InCancellationToken);
	}

	[[nodiscard]] FileReference WithExtensions(String InExtensions) const;
	[[nodiscard]] DirectoryReference GetDirectory() const;

	FileReference& operator =(const FileReference& Rhs) noexcept = default;
	FileReference& operator =(FileReference&& Rhs) noexcept = default;
};