// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "FreeType/FreeTypeModule.h"
#include "FreeType/FontFace.h"
#include "LogFreeType.h"
#include "FreeTypeModule.Impl.h"
#include "FontFace.Impl.h"
#include "Misc/Paths.h"

FreeTypeModule::FreeTypeModule() : Super()
	, _impl(new Impl())
{
	if (FT_Error errorCode = FT_Init_FreeType(&_impl->Library); errorCode)
	{
		SE_LOG(LogFreeType, Fatal, L"Could not initialize FreeType library with error code: {}.", (int32)errorCode);
		return;
	}

	_impl->bInit = true;
}

FreeTypeModule::~FreeTypeModule()
{
	if (_impl->bInit)
	{
		FT_Done_FreeType(_impl->Library);
		_impl->bInit = false;
	}
}

FontFace* FreeTypeModule::CreateFontFace(std::filesystem::path fontFace, int32 faceIndex)
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

	auto faceImpl = std::make_unique<FontFace::Impl>();
	if (FT_Error errorCode = FT_New_Face(_impl->Library, fontFace_str.c_str(), (FT_Long)faceIndex, &faceImpl->Face); errorCode)
	{
		SE_LOG(LogFreeType, Error, L"Could not load font-face on FreeType with error code: {}", (int32)errorCode);
		return nullptr;
	}

	return NewObject<FontFace>(std::move(faceImpl), fontFace.filename());
}