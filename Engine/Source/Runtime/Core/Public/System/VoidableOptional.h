// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "System/IntegralTypes.h"
#include "System/AssertionMacros.h"
#include <optional>

template<class T>
class VoidableOptional
{
	bool bHasValue;
	int8 Buf[sizeof(T)];

public:
	constexpr VoidableOptional() noexcept
		: bHasValue(false)
	{
	}

	constexpr VoidableOptional(std::nullptr_t) noexcept
		: bHasValue(false)
	{
	}

	template<class U>
	constexpr VoidableOptional(U&& InValue) noexcept
		: bHasValue(true)
	{
		new(Buf) T(std::forward<U>(InValue));
	}

	constexpr VoidableOptional(const VoidableOptional& Rhs) noexcept(noexcept(new(Buf) T(Rhs.GetValue())))
		: bHasValue(Rhs.bHasValue)
	{
		if (Rhs.bHasValue)
		{
			new(Buf) T(Rhs.GetValue());
		}
	}

	constexpr VoidableOptional(VoidableOptional&& Rhs) noexcept
		: bHasValue(Rhs.bHasValue)
	{
		if (Rhs.bHasValue)
		{
			new(Buf) T(std::move(Rhs.Buf));
			Rhs.bHasValue = false;
		}
	}

	constexpr ~VoidableOptional() noexcept
	{
		Reset();
	}

	constexpr VoidableOptional& Reset() noexcept
	{
		if (bHasValue)
		{
			GetValue().~T();
			bHasValue = false;
		}
		return *this;
	}

	template<class U>
	constexpr VoidableOptional& SetValue(U&& InValue) noexcept(noexcept(new(Buf) T(std::forward<U>(InValue))))
	{
		Reset();
		new(Buf) T(std::forward<U>(InValue));
		bHasValue = true;
		return *this;
	}

	template<class... U>
	constexpr VoidableOptional& Emplace(U&&... InArgs) noexcept(noexcept(new(Buf) T(std::forward<U>(InArgs)...)))
	{
		Reset();
		new(Buf) T(std::forward<U>(InArgs)...);
		bHasValue = true;
		return *this;
	}

	constexpr bool HasValue() const noexcept
	{
		return bHasValue;
	}

	constexpr T& GetValue() noexcept
	{
		check(bHasValue);
		return *(T*)Buf;
	}

	constexpr const T& GetValue() const noexcept
	{
		check(bHasValue);
		return *(const T*)Buf;
	}

	constexpr bool operator ==(const VoidableOptional& Rhs) const noexcept(noexcept(GetValue() == Rhs.GetValue()))
	{
		if (bHasValue != Rhs.bHasValue)
		{
			return false;
		}
		else if (!bHasValue)
		{
			return true;
		}
		else
		{
			return GetValue() == Rhs.GetValue();
		}
	}

	constexpr T& operator *() noexcept
	{
		return GetValue();
	}

	constexpr VoidableOptional& operator =(const VoidableOptional& Rhs) noexcept(noexcept(new(Buf) T(Rhs.GetValue())))
	{
		Reset();
		bHasValue = Rhs.bHasValue;
		if (Rhs.bHasValue)
		{
			new(Buf) T(Rhs.GetValue());
		}
		return *this;
	}

	constexpr VoidableOptional& operator =(VoidableOptional&& Rhs) noexcept
	{
		Reset();
		bHasValue = Rhs.bHasValue;
		if (Rhs.bHasValue)
		{
			new(Buf) T(std::move(Rhs.Buf));
			Rhs.bHasValue = false;
		}
		return *this;
	}

	constexpr const T& operator *() const noexcept
	{
		return GetValue();
	}

	constexpr T* operator ->() noexcept
	{
		return &GetValue();
	}

	constexpr const T* operator ->() const noexcept
	{
		return &GetValue();
	}
};

template<class T>
VoidableOptional(T&&) -> VoidableOptional<std::remove_const_t<std::remove_reference_t<T>>>;

template<>
class VoidableOptional<void>
{
	bool bHasValue;

private:
	constexpr VoidableOptional(int) noexcept
		: bHasValue(true)
	{
	}

public:
	constexpr VoidableOptional() noexcept
		: bHasValue(false)
	{
	}

	constexpr VoidableOptional(std::nullopt_t) noexcept
		: bHasValue(false)
	{
	}

	constexpr VoidableOptional(const VoidableOptional& Rhs) noexcept
		: bHasValue(Rhs.bHasValue)
	{
	}

	constexpr VoidableOptional(VoidableOptional&& Rhs) noexcept
		: bHasValue(Rhs.bHasValue)
	{
		Rhs.bHasValue = false;
	}

	constexpr VoidableOptional& Reset() noexcept
	{
		bHasValue = false;
		return *this;
	}

	constexpr VoidableOptional& SetValue() noexcept
	{
		bHasValue = true;
		return *this;
	}

	constexpr VoidableOptional& Emplace() noexcept
	{
		bHasValue = true;
		return *this;
	}

	constexpr bool HasValue() const noexcept
	{
		return bHasValue;
	}

	constexpr void GetValue() const noexcept
	{
		check(bHasValue);
	}

	constexpr bool operator ==(const VoidableOptional& Rhs) const noexcept
	{
		return bHasValue == Rhs.bHasValue;
	}

	static constexpr VoidableOptional Completed() noexcept
	{
		return VoidableOptional(0);
	}

	static constexpr VoidableOptional Failure() noexcept
	{
		return VoidableOptional();
	}
};