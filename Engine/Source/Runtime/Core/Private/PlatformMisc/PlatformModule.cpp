// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>
#include "PlatformMisc/PlatformModule.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "LogCore.h"

GENERATE_BODY(SPlatformModule);

SPlatformModule::SPlatformModule(const std::filesystem::path& modulePath)
{
	std::wstring wsPath = modulePath.wstring();
	_NativeHandle = LoadLibraryW(wsPath.c_str());

	if (_NativeHandle == nullptr)
	{
		SE_LOG(LogCore, Error, L"Could not load library from path: {}", wsPath);
		return;
	}

	SetName(modulePath.wstring());
}

SPlatformModule::~SPlatformModule()
{
	if (_NativeHandle)
	{
		FreeLibrary((HMODULE)_NativeHandle);
		_NativeHandle = nullptr;
	}
}

bool SPlatformModule::IsValid() const
{
	return _NativeHandle;
}

void(*SPlatformModule::InternalGetFunctionPointer(std::string_view functionName) const)()
{
	if (!IsValid())
	{
		SE_LOG(LogCore, Error, L"Module not be initialized.");
		return nullptr;
	}

	return reinterpret_cast<void(*)()>(::GetProcAddress((HMODULE)_NativeHandle, functionName.data()));
}