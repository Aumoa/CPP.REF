// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "Launch.h"

#pragma push_macro("TEXT")
#undef TEXT

#include <Windows.h>

#pragma pop_macro("TEXT")

class NWindowsLaunch : public NLaunch
{
	using Super = NLaunch;

public:
	NWindowsLaunch(String CmdArgs) : Super(CmdArgs)
	{
	}
};

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE _, LPWSTR lpCmdLine, INT nShow)
{
	auto WindowsLaunch = std::make_unique<NWindowsLaunch>(String::FromLiteral(lpCmdLine));
	return WindowsLaunch->GuardedMain();
}

#endif