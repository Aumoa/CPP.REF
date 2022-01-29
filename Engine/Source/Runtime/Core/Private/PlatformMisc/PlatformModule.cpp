// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "PlatformMisc/PlatformModule.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "LogCore.h"

#if PLATFORM_WINDOWS
#include "WindowsPlatformCommon.h"

PlatformModule::PlatformModule(const std::filesystem::path& InModulePath)
{
	std::wstring wsPath = InModulePath.wstring();
	NativeHandle = LoadLibraryW(wsPath.c_str());
	ModuleName = InModulePath.stem().wstring();

	if (NativeHandle == nullptr)
	{
		SE_LOG(LogCore, Error, L"Could not load library from path: {}", wsPath);
		return;
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
		SE_LOG(LogCore, Error, L"Module not be initialized.");
		return nullptr;
	}

	return reinterpret_cast<void(*)()>(::GetProcAddress((HMODULE)NativeHandle, FunctionName.data()));
}

#endif