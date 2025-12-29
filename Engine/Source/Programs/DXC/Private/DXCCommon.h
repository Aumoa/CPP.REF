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

namespace Ayla
{
	class HRException : public InvalidOperationException
	{
	public:
		HRException(HRESULT hr, String message)
			: InvalidOperationException(message)
			, HResult(hr)
		{
		}

		const HRESULT HResult;
	};
}

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
	// IDxcIncludeHandler: "7f61fc7d-950d-467f-b3e3-3c02fb49187c"
	static constexpr GUID IID_IDxcIncludeHandler = {
		0x7f61fc7d, 0x950d, 0x467f,
		{0xb3, 0xe3, 0x3c, 0x02, 0xfb, 0x49, 0x18, 0x7c}
	};

	// IDxcCompiler3: "228B4687-5A6A-4730-900C-9702B2203F54"
	static constexpr GUID IID_IDxcCompiler3 = {
		0x228b4687, 0x5a6a, 0x4730,
		{0x90, 0x0c, 0x97, 0x02, 0xb2, 0x20, 0x3f, 0x54}
	};

	// IDxcUtils: "4605C4CB-2019-492A-ADA4-65F20BB7D67F"
	static constexpr GUID IID_IDxcUtils = {
		0x4605c4cb, 0x2019, 0x492a,
		{0xad, 0xa4, 0x65, 0xf2, 0x0b, 0xb7, 0xd6, 0x7f}
	};

	// IDxcBlobEncoding: "7241d424-2646-4191-97c0-98e96e42fc68"
	static constexpr GUID IID_IDxcBlobEncoding = {
		0x7241d424, 0x2646, 0x4191,
		{0x97, 0xc0, 0x98, 0xe9, 0x6e, 0x42, 0xfc, 0x68}
	};

	// IDxcResult: "58346CDA-DDE7-4497-9461-6F87AF5E0659"
	static constexpr GUID IID_IDxcResult = {
		0x58346cda, 0xdde7, 0x4497,
		{0x94, 0x61, 0x6f, 0x87, 0xaf, 0x5e, 0x06, 0x59}
	};

	// IDxcBlob: "8BA5FB08-5195-40e2-AC58-0D989C3A0102"
	static constexpr GUID IID_IDxcBlob = {
		0x8ba5fb08, 0x5195, 0x40e2,
		{0xac, 0x58, 0x0d, 0x98, 0x9c, 0x3a, 0x01, 0x02}
	};

	// IDxcBlobUtf8: "3DA636C9-BA71-4024-A301-30CBF125305B"
	static constexpr GUID IID_IDxcBlobUtf8 = {
		0x3da636c9, 0xba71, 0x4024,
		{0xa3, 0x01, 0x30, 0xcb, 0xf1, 0x25, 0x30, 0x5b}
	};
}

#undef __ALLOW_PLATFORM_COMMON_H__