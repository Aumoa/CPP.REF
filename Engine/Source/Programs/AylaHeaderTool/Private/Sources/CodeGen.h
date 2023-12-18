// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct CodeGen
{
	static constexpr String HeaderComment = TEXT("// This header file is generated by AylaHeaderTool.\n\
// Do NOT modify this file manually.");

	struct Cpp
	{
		static String Include(String IncludePath)
		{
			return String::Format(TEXT("#include \"{0}\""), IncludePath.Replace(TEXT("\\"), TEXT("/")));
		}

		template<class... TIncludes>
		static String Includes(TIncludes&&... Includes)
		{
			std::array<String, sizeof...(TIncludes)> Arr = { Includes... };
			return String::Join(TEXT("\n"), Arr | Linq::Select(Include));
		}
	};
};