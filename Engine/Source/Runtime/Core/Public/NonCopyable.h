// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

namespace Ayla
{
	struct NonCopyable
	{
		NonCopyable() = default;
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable(NonCopyable&&) = default;

		NonCopyable& operator =(const NonCopyable&) = delete;
		NonCopyable& operator =(NonCopyable&&) = default;
	};
}