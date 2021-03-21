// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Path.h"

#include <sstream>
#include "ArgumentNullException.h"

using namespace std;

TRefPtr<String> Path::GetExtension(TRefPtr<String> path)
{
	CheckInvalidPathChars(path);

	const int32 length = (int32)path->Length;
	for (int32 i = length; --i >= 0;)
	{
		const wchar_t ch = path[i];
		// Find first dot that separator of name and extension from last.
		if (ch == ExtensionSeparatorChar)
		{
			if (i != length - 1)
			{
				return path->Substring(i);
			}
			else
			{
				return String::Empty;
			}
		}

		// If find character is directory or volume spearator, the path is have not extension.
		if (ch == DirectorySeparatorChar || ch == AltDirectorySeparatorChar || ch == VolumeSeparatorChar)
		{
			break;
		}
	}

	return String::Empty;
}

TRefPtr<String> Path::ChangeExtension(TRefPtr<String> path, TRefPtr<String> extension)
{
	CheckInvalidPathChars(path);

	const int32 length = (int32)path->Length;
	TRefPtr<String> s = path;
	for (int32 i = length; --i >= 0;)
	{
		const wchar_t ch = path[i];
		// Find first dot that separator of name and extension from last.
		if (ch == ExtensionSeparatorChar)
		{
			s = path->Substring(0, (size_t)i);
			break;
		}

		// If find character is directory or volume spearator, the path is have not extension.
		if (ch == DirectorySeparatorChar || ch == AltDirectorySeparatorChar || ch == VolumeSeparatorChar)
		{
			break;
		}
	}

	wstringstream wss;
	wss << s->C_Str;
	if (extension.IsValid)
	{
		// If extension argument is not start with extension separator character.
		if (extension->Length == 0 || extension[0] != ExtensionSeparatorChar)
		{
			wss << ExtensionSeparatorChar;
		}

		wss << extension->C_Str;
	}

	return wss.str();
}

void Path::CheckInvalidPathChars(TRefPtr<String> path, bool bCheckAdditional)
{
	if (!path.IsValid)
	{
		throw ArgumentNullException(L"path");
	}

	if (HasIllegalCharacters(path, bCheckAdditional))
	{
		throw ArgumentException(L"Invalid path chars.");
	}
}

bool Path::HasIllegalCharacters(TRefPtr<String> path, bool bCheckAdditional)
{
	return path->IndexOfAny(InvalidPathChars, _countof(InvalidPathChars)) || (bCheckAdditional && AnyPathHasWildCardCharacters(path));
}

bool Path::AnyPathHasWildCardCharacters(TRefPtr<String> path, int32 startIndex)
{
	return path->IndexOfAny(WildCardChars, _countof(WildCardChars)).has_value();
}