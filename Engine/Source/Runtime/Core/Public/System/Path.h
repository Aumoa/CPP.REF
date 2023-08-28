// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/StaticClass.h"
#include "System/String.h"
#include <filesystem>

struct Path : public StaticClass
{
	static FORCEINLINE String GetFileName(String InPath)
	{
		return String(InPath.path().filename().wstring());
	}

	static FORCEINLINE String GetFileNameWithoutExtension(String InPath)
	{
		return String(InPath.path().stem().wstring());
	}

	static FORCEINLINE bool HasExtension(String InPath)
	{
		return InPath.path().has_extension();
	}

	static FORCEINLINE String ChangeExtension(String InPath, String InExtension)
	{
		return String(InPath.path().replace_extension(InExtension.path()).wstring());
	}

	static FORCEINLINE String GetRelativePath(String RelativeTo, String InPath)
	{
		return String(std::filesystem::relative(RelativeTo.path(), InPath.path()).wstring());
	}

	static FORCEINLINE String GetFullPath(String InPath)
	{
		return String(std::filesystem::absolute(InPath.path()).wstring());
	}

	static FORCEINLINE String GetDirectoryName(String InPath)
	{
		return String(InPath.path().parent_path().wstring());
	}

	static FORCEINLINE String GetExtension(String InPath)
	{
		return String(InPath.path().extension().wstring());
	}

	template<std::ranges::input_range R> requires std::convertible_to<std::ranges::range_value_t<R>, String>
	static FORCEINLINE String Combine(const R& InRange)
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
	static FORCEINLINE String Combine(const Seq&... InSeq)
	{
		return Combine(std::array{ (String)InSeq... });
	}
};