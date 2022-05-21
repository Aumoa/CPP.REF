// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/PlatformMacros.h"
#include "Misc/PlatformModule.h"
#include "Misc/String.h"
#include "Misc/Path.h"
#include "Exceptions/InvalidOperationException.h"

using namespace libty;

#if PLATFORM_DYNAMIC_LIBRARY

#if PLATFORM_WINDOWS

#pragma push_macro("TEXT")
#undef TEXT

#include <Windows.h>

#pragma pop_macro("TEXT")

PlatformModule::PlatformModule(const String& InModulePath)
{
	NativeHandle = LoadLibraryW((const wchar_t*)InModulePath);
	ModuleName = String(Path::GetFileNameWithoutExtension(InModulePath));

	if (NativeHandle == nullptr)
	{
		throw InvalidOperationException(String::Format(TEXT("Could not load library from path: {}"), InModulePath));
	}
}

PlatformModule::~PlatformModule() noexcept
{
	if (NativeHandle)
	{
		FreeLibrary((HMODULE)NativeHandle);
		NativeHandle = nullptr;
	}
}

String PlatformModule::ToString()
{
	return ModuleName;
}

bool PlatformModule::IsValid() const
{
	return NativeHandle;
}

auto PlatformModule::InternalGetFunctionPointer(const String& FunctionName) const -> FunctionPointer<void>
{
	if (!IsValid())
	{
		throw InvalidOperationException(TEXT("Module not be initialized."));
	}

	std::string Astr = (std::string)FunctionName;
	return reinterpret_cast<FunctionPointer<void>>(::GetProcAddress((HMODULE)NativeHandle, Astr.data()));
}

#endif

#else

PlatformModule::PlatformModule(const String& InModulePath)
{
	throw InvalidOperationException(TEXT("PlatformModule must be with PLATFORM_DYNAMIC_LIBRARY."));
}

PlatformModule::~PlatformModule() noexcept
{
}

String PlatformModule::ToString()
{
	throw InvalidOperationException(TEXT("PlatformModule must be with PLATFORM_DYNAMIC_LIBRARY."));
}

bool PlatformModule::IsValid() const
{
	throw InvalidOperationException(TEXT("PlatformModule must be with PLATFORM_DYNAMIC_LIBRARY."));
}

auto PlatformModule::InternalGetFunctionPointer(const String& FunctionName) const -> FunctionPointer<void>
{
	throw InvalidOperationException(TEXT("PlatformModule must be with PLATFORM_DYNAMIC_LIBRARY."));
}

#endif