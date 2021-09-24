// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Threading/EventHandle.h"

#define NOMINMAX

#pragma push_macro("interface")
#undef interface

#include <comdef.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include <string>
#include <atomic>
#include <queue>
#include <span>
#include <optional>
#include <vector>
#include <bit>

#include <ft2build.h>
#include FT_FREETYPE_H

inline const char* FT_Error_String(FT_Error error)
{
#undef FTERRORS_H_
#define FT_ERRORDEF(error_code, value, string) case error_code: return string;
#define FT_ERROR_START_LIST switch(error) {
#define FT_ERROR_END_LIST default: return "Unknown error"; }
#include FT_ERRORS_H
}

#pragma pop_macro("interface")