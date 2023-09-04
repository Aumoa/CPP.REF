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

public: /* Internal only. */
	void* ApplicationPointer = nullptr;

public:
	NWindowsLaunch(String CmdArgs) : Super(CmdArgs)
	{
	}

	virtual void* GetApplicationPointer() override
	{
		return ApplicationPointer;
	}
};

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE, LPWSTR lpCmdLine, INT nShow)
{
	auto Launch = NLaunch::GeneratePlatformLaunch(String::FromLiteral(lpCmdLine));
	auto* WindowsLaunch = static_cast<NWindowsLaunch*>(Launch.get());
	WindowsLaunch->ApplicationPointer = hInstance;
	return Launch->GuardedMain();
}

std::unique_ptr<NLaunch> NLaunch::GeneratePlatformLaunch(String CmdArgs)
{
	return std::make_unique<NWindowsLaunch>(CmdArgs);
}

#endif