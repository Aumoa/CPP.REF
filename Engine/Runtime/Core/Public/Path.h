// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Core/Object.h"
#include "TRefPtr.h"

class String;

class CORE_API Path abstract : virtual public Object
{
public:
	using Super = Object;
	
public:
	static constexpr const wchar_t DirectorySeparatorChar = L'\\';
	static constexpr const wchar_t AltDirectorySeparatorChar = L'/';
	static constexpr const wchar_t VolumeSeparatorChar = L':';
	static constexpr const wchar_t ExtensionSeparatorChar = L'.';

	static constexpr const wchar_t WildCardChars[] = { L'*', L'?' };
	static constexpr const wchar_t InvalidPathChars[]
	{
		L'\"', L'<', L'>', L'|', L'\0',
		(wchar_t)1, (wchar_t)2, (wchar_t)3, (wchar_t)4, (wchar_t)5, (wchar_t)6, (wchar_t)7, (wchar_t)8, (wchar_t)9, (wchar_t)10,
		(wchar_t)11, (wchar_t)12, (wchar_t)13, (wchar_t)14, (wchar_t)15, (wchar_t)16, (wchar_t)17, (wchar_t)18, (wchar_t)19, (wchar_t)20,
		(wchar_t)21, (wchar_t)22, (wchar_t)23, (wchar_t)24, (wchar_t)25, (wchar_t)26, (wchar_t)27, (wchar_t)28, (wchar_t)29, (wchar_t)30,
		(wchar_t)31
	};

public:
	static TRefPtr<String> GetExtension(TRefPtr<String> path);
	static TRefPtr<String> ChangeExtension(TRefPtr<String> path, TRefPtr<String> extension);

private:
	static void CheckInvalidPathChars(TRefPtr<String> path, bool bCheckAdditional = false);
	static bool HasIllegalCharacters(TRefPtr<String> path, bool bCheckAdditional = false);
	static bool AnyPathHasWildCardCharacters(TRefPtr<String> path, int32 startINdex = 0);
};