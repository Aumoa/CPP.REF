// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/String.h"
#include <filesystem>

class Path
{
public:
	static String GetFileNameWithoutExtension(const String& path)
	{
		return String(std::filesystem::path((std::wstring_view)path).stem().wstring());
	}

	static bool HasExtension(const String& path)
	{
		return std::filesystem::path((const wchar_t*)path).has_extension();
	}

	static String ChangeExtension(const String& path, const String& extension)
	{
		std::filesystem::path p((const wchar_t*)path);
		p.replace_extension(std::filesystem::path((const wchar_t*)extension));
		return String(p.wstring());
	}
};