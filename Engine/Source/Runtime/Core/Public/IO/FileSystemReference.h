// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/String.h"
#include "System/Path.h"

class CORE_API FileSystemReference
{
private:
	String Value;
	String FileName;
	bool bHasExtensions;
	String Name;
	String Extensions;

protected:
	FileSystemReference(String InPath)
	{
		Value = InPath;
		FileName = GetFileName(GetAbsolutePath());
		bHasExtensions = ExtractExtensions(FileName, &Name, &Extensions);
	}

public:
	[[nodiscard]] inline String GetValue() const noexcept
	{
		return Value;
	}

	[[nodiscard]] inline String ToString() const noexcept
	{
		return Value;
	}

	[[nodiscard]] inline String GetFileName() const noexcept
	{
		return FileName;
	}

	[[nodiscard]] inline String GetName() const noexcept
	{
		return Name;
	}

	[[nodiscard]] inline String GetExtensions() const noexcept
	{
		return Extensions;
	}

	[[nodiscard]] inline bool HasExtensions() const noexcept
	{
		return bHasExtensions;
	}

	[[nodiscard]] inline String GetAbsolutePath() const
	{
		return Path::GetFullPath(Value);
	}

	[[nodiscard]] virtual bool IsExists() const = 0;

	[[nodiscard]] inline bool IsPathFullQualified() const
	{
		return Path::IsPathFullQualified(Value);
	}

	[[nodiscard]] bool Includes(const FileSystemReference& Fr) const
	{
		if (Fr.Value.StartsWith(Value, EStringComparison::CurrentCultureIgnoreCase))
		{
			return true;
		}

		if (Fr.GetAbsolutePath().StartsWith(GetAbsolutePath(), EStringComparison::CurrentCultureIgnoreCase))
		{
			return true;
		}

		return false;
	}

	[[nodiscard]] String GetRelativePath(const FileSystemReference& Fr) const
	{
		return Path::GetRelativePath(Fr.Value, Value);
	}

	[[nodiscard]] bool Equals(const FileSystemReference& Rhs) const
	{
		if (Value.Equals(Rhs.Value, EStringComparison::CurrentCultureIgnoreCase))
		{
			return true;
		}

		if (GetAbsolutePath().Equals(Rhs.GetAbsolutePath(), EStringComparison::CurrentCultureIgnoreCase))
		{
			return true;
		}

		return false;
	}

protected:
	[[nodiscard]] static String TrimPath(String InPath)
	{
		return InPath.Trim(' ', '\t', Path::DirectorySeparatorChar);
	}

private:
	[[nodiscard]] static String GetFileName(String InPath)
	{
		InPath = TrimPath(InPath);

		size_t IndexOf = InPath.LastIndexOf(Path::DirectorySeparatorChar);
		if (IndexOf == -1)
		{
			return InPath;
		}

		size_t NextPos = IndexOf + 1;
		if (InPath.IsValidIndex(NextPos) == false)
		{
			return String::GetEmpty();
		}

		return InPath.Substring(NextPos);
	}

	static bool ExtractExtensions(String InFileName, String* OutNamePart, String* OutExtensionsPart)
	{
		size_t IndexOf = InFileName.IndexOf('.');
		if (IndexOf == -1)
		{
			*OutNamePart = InFileName;
			*OutExtensionsPart = String::GetEmpty();
			return false;
		}

		*OutNamePart = InFileName.Substring(0, IndexOf);
		++IndexOf;
		*OutExtensionsPart = InFileName.IsValidIndex(IndexOf) ? InFileName.Substring(IndexOf) : String::GetEmpty();
		return true;
	}

public:
	[[nodiscard]] operator bool() const { return IsExists(); }
	[[nodiscard]] operator String() const { return Value; }

	[[nodiscard]] bool operator ==(const FileSystemReference& Rhs) const
	{
		return Equals(Rhs);
	}

	[[nodiscard]] auto operator <=>(const FileSystemReference& Rhs) const
	{
		return Value <=> Rhs.Value;
	}
};