// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Environment.h"
#include <filesystem>

#include "Misc/WindowsPlatformMisc.h"

String Environment::GetCurrentDirectory()
{
	return String(std::filesystem::current_path().wstring());
}

String Environment::GetExecutingApp()
{
#if PLATFORM_WINDOWS
	static thread_local WCHAR sBuf[1024];
	GetModuleFileNameW(nullptr, sBuf, 1024);
	return String(sBuf);
#endif
}

void Environment::SetCurrentDirectory(String directory)
{
	std::filesystem::current_path((std::wstring)directory);
}