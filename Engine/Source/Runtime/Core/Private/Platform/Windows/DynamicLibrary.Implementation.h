// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/DynamicLibrary.h"
#include "Platform/PlatformCommon.h"

namespace Ayla
{
	class DynamicLibrary::Implementation
	{
		HMODULE hModule = NULL;

	public:
		inline Implementation(String InLibraryName)
		{
			InLibraryName += TEXT(".dll");
			hModule = LoadLibraryW(InLibraryName.c_str());
		}

		inline ~Implementation() noexcept
		{
			if (hModule != NULL)
			{
				FreeLibrary(hModule);
				hModule = NULL;
			}
		}

		inline bool IsValid() const noexcept
		{
			return hModule != NULL;
		}

		inline void (*LoadFunction(String Signature))()
		{
			return reinterpret_cast<void(*)()>(GetProcAddress(hModule, Signature.AsCodepage().c_str()));
		}
	};
}

#endif