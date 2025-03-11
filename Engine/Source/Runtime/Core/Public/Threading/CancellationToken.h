// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include <stop_token>

struct CORE_API CancellationToken
{
private:
	friend class CancellationTokenSource;
	std::stop_token token;

public:
	inline CancellationToken() noexcept
	{
	}

	inline CancellationToken(const CancellationToken& rhs) noexcept
		: token(rhs.token)
	{
	}

	inline CancellationToken(CancellationToken&& rhs) noexcept
		: token(std::move(rhs.token))
	{
	}

	inline bool IsCancellationRequested() const noexcept
	{
		return token.stop_requested();
	}

	inline bool CanBeCanceled() const noexcept
	{
		return token.stop_possible();
	}

	inline void ThrowIfCancellationRequested() const
	{
		if (token.stop_requested())
		{
			ThrowOperationCanceledException();
		}
	}

	inline CancellationToken& operator =(const CancellationToken& rhs) noexcept
	{
		token = rhs.token;
		return *this;
	}

	inline CancellationToken& operator =(CancellationToken&& rhs) noexcept
	{
		token = std::move(rhs.token);
		return *this;
	}

	inline bool operator ==(const CancellationToken& rhs) const noexcept
	{
		return token == rhs.token;
	}

	inline explicit operator std::stop_token& () noexcept
	{
		return token;
	}

	inline explicit operator const std::stop_token& () const noexcept
	{
		return token;
	}

private:
	[[noreturn]]
	static void ThrowOperationCanceledException();
};