// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "Platform/PlatformProcess.h"

#if DO_CHECK
#define check(Expr)						PLATFORM_UNREFERENCED_PARAMETER(Expr)
#define checkf(Expr, Msgf, ...)			PLATFORM_UNREFERENCED_PARAMETER(Expr)
#define ensure(Expr)					(Expr)
#define ensureMsgf(Expr, Msgf, ...)		(Expr)
#else

namespace AssertionMacros::details
{
	template<class TReturn, class TVerify>
	TReturn FORCEINLINE DispatchCheckVerify(TVerify&& Verify) requires std::convertible_to<std::invoke_result_t<TVerify>, TReturn>
	{
		return Verify();
	}
}

#define AE_CHECK_IMPL(Capture, Expr, Msg) \
	(LIKELY(!!(Expr)) || (AssertionMacros::details::DispatchCheckVerify<bool>([Capture]() FORCEINLINE_LAMBDA \
	{ \
		PlatformProcess::OutputDebugString(Msg + TEXT("\n")); \
		return true; \
	}) && ([] () { PLATFORM_BREAK(); } (), false)))

#define check(Expr)						AE_CHECK_IMPL( , Expr, TEXT(#Expr))
#define checkf(Expr, Msgf, ...)			AE_CHECK_IMPL(&, Expr, String::Format(Msgf __VA_OPT__(,) __VA_ARGS__))

#define AE_ENSURE_IMPL(Capture, Expr, Msg) \
	(LIKELY(!!(Expr)) || (AssertionMacros::details::DispatchCheckVerify<bool>([Capture]() FORCEINLINE_LAMBDA \
	{ \
		static bool bExecuted = false; \
		PlatformProcess::OutputDebugString(Msg + TEXT("\n")); \
		if (UNLIKELY(!bExecuted)) \
		{ \
			bExecuted = true; \
			return PlatformProcess::IsDebuggerPresent(); \
		} \
		return false; \
	}) && ([] () { PLATFORM_BREAK(); } (), false)))

#define ensure(Expr)					AE_ENSURE_IMPL( , Expr, TEXT(#Expr))
#define ensureMsgf(Expr, Msgf, ...)		AE_ENSURE_IMPL(&, Expr, String::Format(Msgf __VA_OPT__(,) __VA_ARGS__))
#endif