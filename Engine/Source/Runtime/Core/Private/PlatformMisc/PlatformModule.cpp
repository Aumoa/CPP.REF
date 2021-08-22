// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "PlatformMisc/PlatformModule.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "LogCore.h"

PlatformModule::PlatformModule(const std::filesystem::path& modulePath)
{
	std::wstring wsPath = modulePath.wstring();
	_NativeHandle = LoadLibraryW(wsPath.c_str());

	if (_NativeHandle == nullptr)
	{
		SE_LOG(LogCore, Error, L"Could not load library from path: {}", wsPath);
		return;
	}
}

PlatformModule::~PlatformModule()
{
	if (_NativeHandle)
	{
		FreeLibrary((HMODULE)_NativeHandle);
		_NativeHandle = nullptr;
	}
}

bool PlatformModule::IsValid() const
{
	return _NativeHandle;
}

void(*PlatformModule::InternalGetFunctionPointer(std::string_view functionName) const)()
{
	if (!IsValid())
	{
		SE_LOG(LogCore, Error, L"Module not be initialized.");
		return nullptr;
	}

	return reinterpret_cast<void(*)()>(::GetProcAddress((HMODULE)_NativeHandle, functionName.data()));
}