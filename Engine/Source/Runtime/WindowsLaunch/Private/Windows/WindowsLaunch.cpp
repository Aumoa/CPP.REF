// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "Windows/WindowsLaunch.h"
#include "Platform/PlatformCommon.h"

namespace Ayla
{
	WindowsLaunch::WindowsLaunch(std::vector<String> CmdArgs) : Super(std::move(CmdArgs))
	{
		ApplicationPointer = GetModuleHandleW(nullptr);
	}

	void* WindowsLaunch::GetApplicationPointer()
	{
		return ApplicationPointer;
	}
}

extern "C"
{
	__declspec(dllexport) int Ayla__WindowsLaunch__StartApplication(wchar_t** args, int length)
	{
		using namespace ::Ayla;

		std::vector<String> cmdArgs((size_t)length);
		for (size_t i = 0; i < length; ++i)
		{
			cmdArgs[i] = String::FromLiteral(args[i]);
		}

		auto launch = Object::New<WindowsLaunch>(std::move(cmdArgs));
		int32 ec = launch->GuardedMain();

		launch = nullptr;
		GC::Collect();
		GC::WaitForCompleteToFinalize();

		return ec;
	}
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif