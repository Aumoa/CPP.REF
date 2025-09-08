// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/DynamicLibrary.h"
#include "Platform/PlatformProcess.h"

namespace Ayla
{
	struct DynamicLibrary::Implementation
	{
		void* hModule = nullptr;

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

	DynamicLibrary::DynamicLibrary(DynamicLibrary&& rhs) noexcept
		: LibraryName(std::move(rhs.LibraryName))
		, Impl(rhs.Impl)
	{
		rhs.Impl = nullptr;
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

	String DynamicLibrary::GetName() const
	{
		return LibraryName;
	}

	void DynamicLibrary::Detach()
	{
		Impl->hModule = nullptr;
		delete Impl;
		Impl = nullptr;
	}

	DynamicLibrary& DynamicLibrary::operator =(DynamicLibrary&& rhs) noexcept
	{
		LibraryName = std::move(rhs.LibraryName);
		Impl = rhs.Impl;
		rhs.Impl = nullptr;
		return *this;
	}

	void (*DynamicLibrary::InternalLoadFunction(String Signature) const)()
	{
		return Impl->LoadFunction(Signature);
	}
}