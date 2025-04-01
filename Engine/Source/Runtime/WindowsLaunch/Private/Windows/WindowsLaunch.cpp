// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "Launch.h"
#include "Platform/PlatformCommon.h"

namespace Ayla
{
	class WindowsLaunch : public Launch
	{
		using Super = Launch;

	private:
		void* ApplicationPointer = nullptr;

	public:
		WindowsLaunch(std::vector<String> CmdArgs) : Super(std::move(CmdArgs))
		{
			ApplicationPointer = GetModuleHandleW(nullptr);
		}

		virtual void* GetApplicationPointer() override
		{
			return ApplicationPointer;
		}
	};
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