// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NRHITextFormat;

struct SLATECORE_API NSlateTextFormatCache
{
	struct NCacheIndex
	{
		String FontFamilyName;
		float FontSize = 0;
		bool bBold = false;
		bool bItalic = false;

		constexpr auto operator <=>(const NCacheIndex&) const = default;
		constexpr bool operator ==(const NCacheIndex&) const = default;
	};

	static Spinlock s_Lock;
	static std::map<NCacheIndex, std::shared_ptr<NRHITextFormat>> s_CachedTextFormats;

	static void Initialize();
	static void Shutdown();

	static std::shared_ptr<NRHITextFormat> GetCachedTextFormat(String InFontFamilyName, float InFontSize, bool bInBold, bool bInItalic);
};