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

extern "C" INT __declspec(dllexport) WindowsStartup(LPCWSTR lpCmdLine)
{
	auto Launch = NLaunch::GeneratePlatformLaunch(String::FromLiteral(lpCmdLine));
	auto* WindowsLaunch = static_cast<NWindowsLaunch*>(Launch.get());
	WindowsLaunch->ApplicationPointer = GetModuleHandleW(nullptr);
	return Launch->GuardedMain();
}

std::unique_ptr<NLaunch> NLaunch::GeneratePlatformLaunch(String CmdArgs)
{
	return std::make_unique<NWindowsLaunch>(CmdArgs);
}

#endif