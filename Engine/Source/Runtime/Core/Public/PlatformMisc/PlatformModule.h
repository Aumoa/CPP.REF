// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include <filesystem>

class SGameInstance;

class CORE_API PlatformModule
{
private:
	void* NativeHandle = nullptr;
	std::wstring ModuleName;

public:
	PlatformModule(const std::filesystem::path& InModulePath);
	~PlatformModule() noexcept;

	std::wstring ToString();
	bool IsValid() const;

	template<class TFunction>
	TFunction* GetFunctionPointer(std::string_view FunctionName) const
	{
		return reinterpret_cast<TFunction*>(InternalGetFunctionPointer(FunctionName));
	}

	template<class TRet, class... TArgs>
	TRet(*GetFunctionPointer(std::string_view FunctionName) const)(TArgs...)
	{
		return reinterpret_cast<TRet(*)(TArgs...)>(InternalGetFunctionPointer(FunctionName));
	}

private:
	void(*InternalGetFunctionPointer(std::string_view FunctionName) const)();
};