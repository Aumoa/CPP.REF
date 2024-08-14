// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <stop_token>
#include "Threading/CancellationToken.h"
#include "System/TimeSpan.h"

class CORE_API CancellationTokenSource
{
	bool hasValue;
	char native[sizeof(std::stop_source)];

	CancellationTokenSource(const CancellationTokenSource&) noexcept = delete;
	CancellationTokenSource& operator =(const CancellationTokenSource&) noexcept = delete;

public:
	inline CancellationTokenSource()
		: hasValue(false)
	{
	}

	inline CancellationTokenSource(std::in_place_t)
		: hasValue(true)
	{
		new(native) std::stop_source;
	}

	inline CancellationTokenSource(CancellationTokenSource&& rhs) noexcept
		: hasValue(rhs.hasValue)
	{
		if (hasValue)
		{
			new(native) std::stop_source(std::move(rhs.get_source()));
			rhs.hasValue = false;
		}
	}

	inline ~CancellationTokenSource() noexcept
	{
		if (hasValue)
		{
			get_source().~stop_source();
			hasValue = false;
		}
	}

	inline CancellationToken GetToken() const
	{
		if (hasValue == false)
		{
			ThrowInvalidOperationException();
		}

		CancellationToken token;
		token.token = get_source().get_token();
		return token;
	}

	inline void Cancel()
	{
		if (hasValue == false)
		{
			ThrowInvalidOperationException();
		}

		get_source().request_stop();
	}

	void CancelAfter(const TimeSpan& delay);

	inline CancellationTokenSource& operator =(CancellationTokenSource&& rhs) noexcept
	{
		if (hasValue)
		{
			get_source().~stop_source();
		}

		hasValue = rhs.hasValue;
		if (hasValue)
		{
			new(native) std::stop_source(std::move(rhs.get_source()));
			rhs.hasValue = false;
		}

		return *this;
	}

	inline bool operator ==(const CancellationTokenSource& rhs) const noexcept
	{
		if (hasValue != rhs.hasValue)
		{
			return false;
		}

		if (hasValue == false)
		{
			return true;
		}

		return get_source() == rhs.get_source();
	}

public:
	static inline CancellationTokenSource Create()
	{
		return CancellationTokenSource(std::in_place);
	}

private:
	inline const std::stop_source& get_source() const noexcept
	{
		return *reinterpret_cast<const std::stop_source*>(native);
	}

	inline std::stop_source& get_source() noexcept
	{
		return *reinterpret_cast<std::stop_source*>(native);
	}

	[[noreturn]]
	static void ThrowInvalidOperationException();
};