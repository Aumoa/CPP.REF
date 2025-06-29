// Copyright 2020-2025 Aumoa.lib. All right reserved.

#define __ALLOW_PLATFORM_COMMON_H__

#include "CoreMinimal.h"
#include "Platform/PlatformCommon.h"
#include "Launch.h"
#include "Platform/DynamicLibrary.h"

int main(int argc, char* argv[])
{
	using namespace ::Ayla;

	try_([]()
	{
        DynamicLibrary hAPI(TEXT("LinuxAPI"));
        if (hAPI.IsValid() == false)
        {
			throw std::runtime_error("Failed to load LinuxAPI");
        }

        hAPI.LoadFunction<

		auto allocator = (::Ayla::ssize_t(*)())GetProcAddress(hLinuxAPI, ACLASS__NAMEOF_NEW_CLASS_INJECTED(Ayla, LinuxLaunch));
		if (allocator == nullptr)
		{
			throw std::runtime_error("Failed to find generated function for create Launch instance in LinuxAPI");
		}

		auto launch = std::shared_ptr<Marshal::IntPtrToRPtrDeleteCopy<Launch>(allocator());
		return launch->StartApplication();
	})
	.finally_([]()
	{
		GC::Collect();
		GC::WaitForCompleteToFinalize();
	});
}