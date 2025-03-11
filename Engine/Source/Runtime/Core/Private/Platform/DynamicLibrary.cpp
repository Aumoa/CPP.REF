// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/DynamicLibrary.h"
#include "Platform/Windows/DynamicLibrary.Implementation.h"

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