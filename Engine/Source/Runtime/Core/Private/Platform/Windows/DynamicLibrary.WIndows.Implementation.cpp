// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#define __ALLOW_PLATFORM_COMMON_H__
#include "Platform/PlatformCommon.h"
#undef __ALLOW_PLATFORM_COMMON_H__

export module Core:DynamicLibrary_Windows_Implementation;

export import :DynamicLibrary;
export import :String;

#if PLATFORM_WINDOWS

DynamicLibrary::Implementation::Implementation(String InLibraryName)
{
	InLibraryName += TEXT(".dll");
	handle = LoadLibraryW(InLibraryName.c_str());
}

DynamicLibrary::Implementation::~Implementation() noexcept
{
	if (handle != NULL)
	{
		FreeLibrary((HMODULE)handle);
		handle = NULL;
	}
}

bool DynamicLibrary::Implementation::IsValid() const noexcept
{
	return handle != NULL;
}

void (*DynamicLibrary::Implementation::LoadFunction(String Signature))()
{
	return reinterpret_cast<void(*)()>(GetProcAddress((HMODULE)handle, Signature.AsCodepage().c_str()));
}

#endif