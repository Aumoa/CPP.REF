// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "FreeType/FontFace.h"

struct SFontFace::Impl
{
	FT_Face Face;
};