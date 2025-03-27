// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "Launch.h"
#include "Platform/PlatformCommon.h"

namespace Ayla
{
	class NWindowsLaunch : public NLaunch
	{
		using Super = NLaunch;

	private:
		void* ApplicationPointer = nullptr;

	public:
		NWindowsLaunch(std::vector<String> CmdArgs) : Super(std::move(CmdArgs))
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

		auto Launch = std::make_shared<NWindowsLaunch>(std::move(cmdArgs));
		return Launch->GuardedMain();
	}
}

INT APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR lpCmdLine, INT)
{
	using namespace Ayla;
	auto Launch = std::make_shared<NWindowsLaunch>(std::vector<String>{});
	return Launch->GuardedMain();
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif