// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "Launch.h"

#pragma push_macro("TEXT")
#undef TEXT

#include <Windows.h>

#pragma pop_macro("TEXT")

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

	std::unique_ptr<NLaunch> NLaunch::GeneratePlatformLaunch(String CmdArgs)
	{
		return std::make_unique<NWindowsLaunch>(CmdArgs);
	}
}

#endif