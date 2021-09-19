// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SFontFace;

struct SlateFont
{
	SFontFace* FontFace = nullptr;
	int32 FontSize = 11;

	inline SlateFont()
	{
	}

	inline SlateFont(SFontFace* fontFace, int32 fontSize)
		: FontFace(fontFace)
		, FontSize(fontSize)
	{
	}
};