// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "LogGame.h"

#pragma push_macro("interface")
#undef interface

#include <comdef.h>
#include <wrl/client.h>
#include <wincodec.h>

using Microsoft::WRL::ComPtr;

#pragma pop_macro("interface")

DECLARE_LOG_CATEGORY(, LogWindows);

class WindowsPlatformHelper abstract final
{
public:
	static std::wstring GetResultString(HRESULT hr);
};

#define HR(X)										\
if (HRESULT hr = (X); FAILED(hr))					\
{													\
	SE_LOG(LogWindows, Fatal,						\
	L"Fatal error in HRESULT: {:08#X}, Message: {}",\
	hr, WindowsPlatformHelper::GetResultString(hr));\
}