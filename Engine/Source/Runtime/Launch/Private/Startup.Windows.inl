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
		HMODULE hWindowsAPI = LoadLibraryW(L"WindowsAPI.dll");
		if (hWindowsAPI == nullptr)
		{
			throw std::runtime_error("Failed to load WindowsAPI.dll");
		}

		auto allocator = (::Ayla::ssize_t(*)())GetProcAddress(hWindowsAPI, ACLASS__NAMEOF_NEW_CLASS_INJECTED(Ayla, WindowsLaunch));
		if (allocator == nullptr)
		{
			throw std::runtime_error("Failed to find generated function for create Launch instance in WindowsAPI.dll");
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