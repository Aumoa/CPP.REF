// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "Launch.h"
#include "Platform/PlatformCommon.h"
#include "WindowsLaunch.gen.h"

namespace Ayla
{
	ACLASS()
	class WindowsLaunch : public Launch
	{
		GENERATED_BODY()

	private:
		void* ApplicationPointer = nullptr;

	public:
		WindowsLaunch();

		void* GetApplicationPointer() override;
	};
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif