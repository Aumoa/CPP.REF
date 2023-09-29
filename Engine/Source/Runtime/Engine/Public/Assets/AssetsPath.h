// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct ENGINE_API NAssetsPath
{
	static String GetFileSystemPath(String InLogicalPath);

	template<std::ranges::input_range R> requires std::convertible_to<std::ranges::range_value_t<R>, String>
	static inline String Combine(const R& InRange)
	{
		return String::Join(DirectorySeparatorString, InRange);
	}

	template<std::convertible_to<String>... Seq>
	static inline String Combine(const Seq&... InSeq)
	{
		return Combine(std::array{ (String)InSeq... });
	}

	static constexpr char_t DirectorySeparatorChar = '/';
	static constexpr String DirectorySeparatorString = TEXT("/");
};