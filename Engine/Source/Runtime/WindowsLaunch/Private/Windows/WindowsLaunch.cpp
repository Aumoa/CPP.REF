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
		NWindowsLaunch(String CmdArgs) : Super(CmdArgs)
		{
			ApplicationPointer = GetModuleHandleW(nullptr);
		}

		virtual void* GetApplicationPointer() override
		{
			return ApplicationPointer;
		}
	};
}

extern "C" INT APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR lpCmdLine, INT)
{
	using namespace Ayla;
	auto Launch = std::make_shared<NWindowsLaunch>(String::FromLiteral(lpCmdLine));
	return Launch->GuardedMain();
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif