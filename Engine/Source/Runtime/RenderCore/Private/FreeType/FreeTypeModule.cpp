// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "FreeType/FreeTypeModule.h"
#include "FreeType/FontFace.h"
#include "FreeType/FontCachingManager.h"
#include "LogFreeType.h"
#include "FreeTypeModule.Impl.h"
#include "FontFace.Impl.h"
#include "Misc/Paths.h"

SFreeTypeModule::SFreeTypeModule() : Super()
	, _impl(new Impl())
{
	if (FT_Error errorCode = FT_Init_FreeType(&_impl->Library); errorCode)
	{
		SE_LOG(LogFreeType, Fatal, L"Could not initialize FreeType library with error code: {}.", (int32)errorCode);
		return;
	}

	_impl->bInit = true;
}

SFreeTypeModule::~SFreeTypeModule()
{
	if (_impl->bInit)
	{
		FT_Done_FreeType(_impl->Library);
		_impl->bInit = false;
	}
}

SFontFace* SFreeTypeModule::CreateFontFace(std::filesystem::path fontFace, int32 faceIndex)
{
	if (!fontFace.has_extension())
	{
		// Make default extension.
		fontFace.replace_extension(L"ttf");
	}

	if (!std::filesystem::exists(fontFace))
	{
		// Search from platform-default font directory.
		fontFace = Paths::GetPlatformFontPath() / fontFace;

		if (!std::filesystem::exists(fontFace))
		{
			SE_LOG(LogFreeType, Error, L"Could not found font file: {}.", fontFace.wstring());
			return nullptr;
		}
	}

	std::string fontFace_str = WCHAR_TO_ANSI(fontFace.wstring());

	auto it = _faces.find(fontFace_str);
	if (it == _faces.end())
	{

		auto faceImpl = std::make_unique<SFontFace::Impl>();
		if (FT_Error errorCode = FT_New_Face(_impl->Library, fontFace_str.c_str(), (FT_Long)faceIndex, &faceImpl->Face); errorCode)
		{
			SE_LOG(LogFreeType, Error, L"Could not load font-face on FreeType with error code: {}", (int32)errorCode);
			return nullptr;
		}

		auto [emplace_it, succeeded] = _faces.emplace(fontFace_str, NewObject<SFontFace>(this, std::move(faceImpl), fontFace.filename()));
		it = emplace_it;
	}

	return it->second;
}

SFontCachingManager* SFreeTypeModule::CreateCachingMgr(SRHIDevice* device)
{
	if (_cachingMgr == nullptr)
	{
		_cachingMgr = NewObject<SFontCachingManager>(device);
	}

	return _cachingMgr;
}