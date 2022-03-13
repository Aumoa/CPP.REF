// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/PlatformMacros.h"

#if PLATFORM_WINDOWS

#include <Windows.h>
#include "Misc/PlatformModule.h"
#include "Misc/Exceptions.h"

PlatformModule::PlatformModule(const std::filesystem::path& InModulePath)
{
	std::wstring wsPath = InModulePath.wstring();
	NativeHandle = LoadLibraryW(wsPath.c_str());
	ModuleName = InModulePath.stem().wstring();

	if (NativeHandle == nullptr)
	{
		throw invalid_operation(std::format(L"Could not load library from path: {}", wsPath));
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

void(*PlatformModule::InternalGetFunctionPointer(std::string_view FunctionName) const)()
{
	if (!IsValid())
	{
		throw invalid_operation("Module not be initialized.");
	}

	return reinterpret_cast<void(*)()>(::GetProcAddress((HMODULE)NativeHandle, FunctionName.data()));
}

#endif