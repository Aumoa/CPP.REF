// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <filesystem>

class CORE_API PlatformModule
{
private:
	template<class TRet, class... TArgs>
	using FunctionPointer = TRet(*)(TArgs...);

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
	FunctionPointer<TRet, TArgs...> GetFunctionPointer(std::string_view FunctionName) const
	{
		return reinterpret_cast<TRet(*)(TArgs...)>(InternalGetFunctionPointer(FunctionName));
	}

private:
	FunctionPointer<void> InternalGetFunctionPointer(std::string_view FunctionName) const;
};