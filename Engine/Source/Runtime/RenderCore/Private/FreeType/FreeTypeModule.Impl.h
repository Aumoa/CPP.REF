// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include "FreeType/FreeTypeModule.h"

struct FreeTypeModule::Impl
{
	bool bInit = false;
	FT_Library Library;
};