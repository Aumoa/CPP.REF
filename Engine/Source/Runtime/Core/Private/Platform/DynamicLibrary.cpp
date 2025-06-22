// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/DynamicLibrary.h"
#include "Platform/PlatformProcess.h"

namespace Ayla
{
	class DynamicLibrary::Implementation
	{
		void* hModule = nullptr;

	public:
		inline Implementation(String InLibraryName)
		{
			hModule = PlatformProcess::LoadLibrary(InLibraryName);
		}

		inline ~Implementation() noexcept
		{
			if (hModule != nullptr)
			{
				PlatformProcess::FreeLibrary(hModule);
				hModule = nullptr;
			}
		}

		inline bool IsValid() const noexcept
		{
			return hModule != nullptr;
		}

		inline void (*LoadFunction(String Signature))()
		{
			return reinterpret_cast<void(*)()>(PlatformProcess::GetLibraryFunction(hModule, Signature));
		}
	};

	DynamicLibrary::DynamicLibrary()
	{
	}

	DynamicLibrary::DynamicLibrary(String InLibraryName)
		: LibraryName(InLibraryName)
		, Impl(new Implementation(InLibraryName))
	{
	}

	DynamicLibrary::~DynamicLibrary() noexcept
	{
		if (Impl)
		{
			delete Impl;
			Impl = nullptr;
		}
	}

	bool DynamicLibrary::IsValid() const
	{
		return !LibraryName.IsEmpty() && Impl != nullptr && Impl->IsValid();
	}

	void (*DynamicLibrary::InternalLoadFunction(String Signature))()
	{
		return Impl->LoadFunction(Signature);
	}
}