// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#define __ALLOW_PLATFORM_COMMON_H__

#include "CoreMinimal.h"
#include "LogDXC.h"
#include "Platform/PlatformCommon.h"

#pragma push_macro("TEXT")
#pragma warning(push)
#pragma warning(disable: 4458)

#undef TEXT

#include <dxcapi.h>
#include "ComPtr.inl"

#undef CreateDirectory

#pragma warning(pop)
#pragma pop_macro("TEXT")

inline void HR__impl(Ayla::String s)
{
	Ayla::LogDXC::Critical(s);
	throw Ayla::InvalidOperationException(s);
}

inline Ayla::String HR__format(HRESULT value)
{
	static thread_local std::map<HRESULT, _com_error> sCache;
	auto it = sCache.find(value);
	if (it == sCache.end())
	{
		it = sCache.emplace(value, value).first;
	}
	const wchar_t* msg = it->second.ErrorMessage();
	return Ayla::String::FromLiteral(msg);
}

#define HR(expr, ...) \
if (auto HR_res__ = (expr); FAILED(HR_res__)) \
{ \
	HR__impl(HR__format(HR_res__)); \
}

namespace Ayla
{
	String ReadLine();
}

#undef __ALLOW_PLATFORM_COMMON_H__