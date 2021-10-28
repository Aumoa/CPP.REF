// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#undef interface
#define NOMINMAX

#include <comdef.h>
#include <Windows.h>
#include <d3d11.h>
#include <d2d1_1.h>
#include <wrl/client.h>

#undef GetCommandLine
using Microsoft::WRL::ComPtr;

#include "WindowsErrors.inl"

#define HR(X)						\
if (HRESULT __E = (X); FAILED(__E))	\
{									\
	ReportCOMError(L ## #X, __E);	\
}