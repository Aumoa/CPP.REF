// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "Platform/PlatformMacros.h"

export module Core:AssertionMacros;

export import :Std;

export namespace AssertionMacros::details
{
	template<class TReturn, class TVerify>
	TReturn FORCEINLINE DispatchCheckVerify(TVerify&& Verify) requires std::convertible_to<std::invoke_result_t<TVerify>, TReturn>
	{
		return Verify();
	}
}