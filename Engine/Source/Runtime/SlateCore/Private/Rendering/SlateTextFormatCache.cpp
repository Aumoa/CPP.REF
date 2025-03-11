// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/SlateTextFormatCache.h"
#include "RHI/RHITextFormat.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"

Spinlock NSlateTextFormatCache::s_Lock;
std::map<NSlateTextFormatCache::NCacheIndex, std::shared_ptr<NRHITextFormat>> NSlateTextFormatCache::s_CachedTextFormats;

void NSlateTextFormatCache::Initialize()
{
}

void NSlateTextFormatCache::Shutdown()
{
	s_CachedTextFormats.clear();
}

std::shared_ptr<NRHITextFormat> NSlateTextFormatCache::GetCachedTextFormat(String InFontFamilyName, float InFontSize, bool bInBold, bool bInItalic)
{
	NCacheIndex CacheIndex;
	CacheIndex.FontFamilyName = InFontFamilyName;
	CacheIndex.FontSize = InFontSize;
	CacheIndex.bBold = bInBold;
	CacheIndex.bItalic = bInItalic;

	auto ScopedLock = std::unique_lock(s_Lock);
	auto It = s_CachedTextFormats.find(CacheIndex);
	if (It != s_CachedTextFormats.end())
	{
		return It->second;
	}

	std::shared_ptr<NRHITextFormat> TextFormat
		= NRHIGlobal::GetDynamicRHI().CreateTextFormat(InFontFamilyName, InFontSize, bInBold, bInItalic);
	s_CachedTextFormats.emplace(CacheIndex, TextFormat);

	return TextFormat;
}