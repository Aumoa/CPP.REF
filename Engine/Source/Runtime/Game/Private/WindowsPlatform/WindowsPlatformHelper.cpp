// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "WindowsPlatformHelper.h"

DEFINE_LOG_CATEGORY(LogWindows);

std::wstring WindowsPlatformHelper::GetResultString(HRESULT hr)
{
	const TCHAR* errorMsg = _com_error(hr).ErrorMessage();
	return TCHAR_TO_WCHAR(errorMsg);
}