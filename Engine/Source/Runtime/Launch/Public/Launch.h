// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class NGenericApplication;

	class LAUNCH_API NLaunch
	{
	private:
		static NLaunch* CurrentLaunch;

		String CmdArgs;
		std::unique_ptr<NGenericApplication> GenericApp;

	protected:
		NLaunch(String CmdArgs);

	public:
		virtual ~NLaunch() noexcept;

	public:
		int32 GuardedMain();

		virtual void* GetApplicationPointer() = 0;

		static NLaunch& Get() noexcept;
		static std::unique_ptr<NLaunch> GeneratePlatformLaunch(String CmdArgs);
	};
}

extern "C" LAUNCH_API ::Ayla::int32 Ayla__Launch__StartApplication(const ::Ayla::char_t* const* args, ::Ayla::int32 length);