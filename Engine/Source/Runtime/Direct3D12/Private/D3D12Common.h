// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#pragma push_macro("TEXT")
#include <d3d12.h>
#include <dxgi1_5.h>
#include <wrl/client.h>
#include <comdef.h>
#include "Platform/Windows/UndefWindowsPlatformMacros.h"
#pragma pop_macro("TEXT")
#include "LogD3D12.h"

using Microsoft::WRL::ComPtr;

inline void HR__impl(Ayla::String s)
{
	Ayla::LogD3D12::Critical(s);
	throw Ayla::InvalidOperationException(s);
}

#define HR(expr, ...) \
if (auto HR_res__ = (expr); FAILED(HR_res__)) \
{ \
	HR__impl(Ayla::String::Format(TEXT("{}"), HR_res__)); \
}

template<>
struct std::formatter<HRESULT, wchar_t> : public std::formatter<Ayla::String, wchar_t>
{
	template<class TFormatContext>
	auto format(HRESULT value, TFormatContext& context) const
	{
		static thread_local std::map<HRESULT, _com_error> sCache;
		auto it = sCache.find(value);
		if (it == sCache.end())
		{
			it = sCache.emplace(value, value).first;
		}
		const wchar_t* msg = it->second.ErrorMessage();
		return std::formatter<Ayla::String, wchar_t>::format(Ayla::String::FromLiteral(msg), context);
	}
};