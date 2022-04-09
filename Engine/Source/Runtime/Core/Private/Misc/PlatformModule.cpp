// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/PlatformMacros.h"
#include "Misc/PlatformModule.h"
#include "Misc/String.h"
#include "Exceptions/InvalidOperationException.h"

using namespace libty;

#if PLATFORM_DYNAMIC_LIBRARY

#if PLATFORM_WINDOWS

#include <Windows.h>

PlatformModule::PlatformModule(const std::filesystem::path& InModulePath)
{
	NativeHandle = LoadLibraryW(InModulePath.wstring().c_str());
	ModuleName = InModulePath.stem().wstring();

	if (NativeHandle == nullptr)
	{
		throw InvalidOperationException(String::Format("Could not load library from path: {}", InModulePath.string()));
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

std::wstring PlatformModule::ToString()
{
	return ModuleName;
}

bool PlatformModule::IsValid() const
{
	return NativeHandle;
}

auto PlatformModule::InternalGetFunctionPointer(std::string_view FunctionName) const -> FunctionPointer<void>
{
	if (!IsValid())
	{
		throw InvalidOperationException("Module not be initialized.");
	}

	return reinterpret_cast<FunctionPointer<void>>(::GetProcAddress((HMODULE)NativeHandle, FunctionName.data()));
}

#endif

#else

PlatformModule::PlatformModule(const std::filesystem::path& InModulePath)
{
	throw InvalidOperationException("PlatformModule must be with PLATFORM_DYNAMIC_LIBRARY.");
}

PlatformModule::~PlatformModule() noexcept
{
}

std::wstring PlatformModule::ToString()
{
	throw InvalidOperationException("PlatformModule must be with PLATFORM_DYNAMIC_LIBRARY.");
}

bool PlatformModule::IsValid() const
{
	throw InvalidOperationException("PlatformModule must be with PLATFORM_DYNAMIC_LIBRARY.");
}

auto PlatformModule::InternalGetFunctionPointer(std::string_view FunctionName) const -> FunctionPointer<void>
{
	throw InvalidOperationException("PlatformModule must be with PLATFORM_DYNAMIC_LIBRARY.");
}

#endif