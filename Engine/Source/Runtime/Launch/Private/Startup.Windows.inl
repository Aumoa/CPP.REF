// Copyright 2020-2025 Aumoa.lib. All right reserved.

#define __ALLOW_PLATFORM_COMMON_H__

#include "CoreMinimal.h"
#include "Platform/PlatformCommon.h"
#include "Launch.h"

INT APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	using namespace ::Ayla;

	try_([]()
	{
		HMODULE hWindowsLaunch = LoadLibraryW(L"WindowsLaunch.dll");
		if (hWindowsLaunch == nullptr)
		{
			throw std::runtime_error("Failed to load WindowsLaunch.dll");
		}

		auto allocator = (::Ayla::ssize_t(*)())GetProcAddress(hWindowsLaunch, ACLASS__NAMEOF_NEW_CLASS_INJECTED(Ayla, WindowsLaunch));
		if (allocator == nullptr)
		{
			throw std::runtime_error("Failed to find generated function for create Launch instance in WindowsLaunch.dll");
		}

		auto launch = Marshal::IntPtrToRPtrDeleteCopy<Launch>(allocator());
		return launch->StartApplication();
	})
	.finally_([]()
	{
		GC::Collect();
		GC::WaitForCompleteToFinalize();
	});
}