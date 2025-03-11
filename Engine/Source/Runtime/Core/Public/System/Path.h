// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "System/StaticClass.h"
#include "System/String.h"
#include <filesystem>

struct Path : public StaticClass
{
	static inline String GetFileName(String InPath)
	{
		return String(InPath.path().filename().wstring());
	}

	static inline String GetFileNameWithoutExtension(String InPath)
	{
		return String(InPath.path().stem().wstring());
	}

	static inline bool HasExtension(String InPath)
	{
		return InPath.path().has_extension();
	}

	static inline String ChangeExtension(String InPath, String InExtension)
	{
		return String(InPath.path().replace_extension(InExtension.path()).wstring());
	}

	static inline String GetRelativePath(String RelativeTo, String InPath)
	{
		return String(std::filesystem::relative(InPath.path(), RelativeTo.path()).wstring());
	}

	static inline String GetFullPath(String InPath)
	{
		return String(std::filesystem::absolute(InPath.path()).wstring());
	}

	static inline String GetDirectoryName(String InPath)
	{
		return String(InPath.path().parent_path().wstring());
	}

	static inline String GetExtension(String InPath)
	{
		return String(InPath.path().extension().wstring());
	}

	static inline bool CompareExtension(String InPath, String Extension)
	{
		return GetExtension(InPath).Equals(Extension, EStringComparison::CurrentCultureIgnoreCase);
	}

	template<std::same_as<String>... TExtensions>
	static inline bool CompareExtensions(String InPath, TExtensions&&... Extensions)
	{
		return (false || ... || CompareExtension(InPath, Extensions));
	}

	static inline bool IsPathFullQualified(String path)
	{
		return path.path().is_absolute();
	}

	template<std::ranges::input_range R> requires std::convertible_to<std::ranges::range_value_t<R>, String>
	static inline String Combine(const R& InRange)
	{
		std::optional<std::filesystem::path> Result;
		for (auto& PathStr : InRange)
		{
			if (Result.has_value() == false)
			{
				Result = PathStr.path();
			}
			else
			{
				Result.value() /= PathStr.path();
			}
		}
		return String(Result.value_or(std::filesystem::path()).wstring());
	}

	template<std::convertible_to<String>... Seq>
	static inline String Combine(const Seq&... InSeq)
	{
		return Combine(std::array{ (String)InSeq... });
	}

	// Platform specified separator char.
	static constexpr char_t DirectorySeparatorChar = std::filesystem::path::preferred_separator;

	// Universal separator char.
	static constexpr char_t AltDirectorySeparatorChar = '/';
};