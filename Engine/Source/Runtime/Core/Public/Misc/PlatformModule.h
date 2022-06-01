// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include <filesystem>

class CORE_API PlatformModule
{
private:
	template<class TRet, class... TArgs>
	using FunctionPointer = TRet(*)(TArgs...);

private:
	void* NativeHandle = nullptr;
	String ModuleName;

public:
	PlatformModule(const String& InModulePath);
	~PlatformModule() noexcept;

	String ToString();
	bool IsValid() const;

	template<class TFunction>
	TFunction* GetFunctionPointer(const String& FunctionName) const
	{
		return reinterpret_cast<TFunction*>(InternalGetFunctionPointer(FunctionName));
	}

	template<class TRet, class... TArgs>
	FunctionPointer<TRet, TArgs...> GetFunctionPointer(const String& FunctionName) const
	{
		return reinterpret_cast<TRet(*)(TArgs...)>(InternalGetFunctionPointer(FunctionName));
	}

private:
	FunctionPointer<void> InternalGetFunctionPointer(const String& FunctionName) const;
};