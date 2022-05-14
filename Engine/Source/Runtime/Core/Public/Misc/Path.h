// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "String.h"
#include <filesystem>

namespace libty::inline Core
{
	class Path
	{
	public:
		static String GetFileNameWithoutExtension(String path)
		{
			return String(std::filesystem::path((std::wstring_view)path).stem().wstring());
		}

		static bool HasExtension(String path)
		{
			return std::filesystem::path((const wchar_t*)path).has_extension();
		}

		static String ChangeExtension(String path, String extension)
		{
			std::filesystem::path p((const wchar_t*)path);
			p.replace_extension(std::filesystem::path((const wchar_t*)extension));
			return String(p.wstring());
		}
	};
}