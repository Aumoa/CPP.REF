// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "System/IntegralTypes.h"
#include <chrono>

using namespace std::chrono_literals;

class TimeSpan
{
	std::chrono::nanoseconds Val;

public:
	static constexpr const int64 TicksPerYear = std::nano::den * std::chrono::years::period::num;
	static constexpr const int64 TicksPerMonth = std::nano::den * std::chrono::months::period::num;
	static constexpr const int64 TicksPerDay = std::nano::den * std::chrono::days::period::num;
	static constexpr const int64 TicksPerHour = std::nano::den * std::chrono::hours::period::num;
	static constexpr const int64 TicksPerMinutes = std::nano::den * std::chrono::minutes::period::num;
	static constexpr const int64 TicksPerSeconds = std::nano::den * std::chrono::seconds::period::num;
	static constexpr const int64 TicksPerMilliseconds = std::nano::den / std::chrono::milliseconds::period::den;
	static constexpr const int64 TicksPerMicroseconds = std::nano::den / std::chrono::microseconds::period::den;
	static constexpr const int64 TicksPerNanoseconds = std::nano::den / std::chrono::nanoseconds::period::den;

public:
	inline constexpr TimeSpan() noexcept
		: Val(0ns)
	{
	}

	inline constexpr TimeSpan(const std::chrono::nanoseconds& InVal) noexcept
		: Val(InVal)
	{
	}

	template<class TRep, class TPeriod>
	inline constexpr TimeSpan(const std::chrono::duration<TRep, TPeriod>& InVal) noexcept
		: Val(std::chrono::duration_cast<std::chrono::nanoseconds>(InVal))
	{
	}

public:
	inline constexpr int64 GetYears() const noexcept
	{
		return std::chrono::floor<std::chrono::years>(Val).count();
	}

	inline constexpr int64 GetMonths() const noexcept
	{
		auto Years = std::chrono::floor<std::chrono::years>(Val);
		return std::chrono::floor<std::chrono::months>(Val - Years).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalMonths() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::months::period>>(Val).count();
	}

	inline constexpr int64 GetDays() const noexcept
	{
		auto Months = std::chrono::floor<std::chrono::months>(Val);
		return std::chrono::floor<std::chrono::days>(Val - Months).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalDays() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::days::period>>(Val).count();
	}

	inline constexpr int64 GetHours() const noexcept
	{
		auto Days = std::chrono::floor<std::chrono::days>(Val);
		return std::chrono::floor<std::chrono::hours>(Val - Days).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalHours() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::hours::period>>(Val).count();
	}

	inline constexpr int64 GetMinutes() const noexcept
	{
		auto Hours = std::chrono::floor<std::chrono::hours>(Val);
		return std::chrono::floor<std::chrono::minutes>(Val - Hours).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalMinutes() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::minutes::period>>(Val).count();
	}

	inline constexpr int64 GetSeconds() const noexcept
	{
		auto Minutes = std::chrono::floor<std::chrono::minutes>(Val);
		return std::chrono::floor<std::chrono::seconds>(Val - Minutes).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalSeconds() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::seconds::period>>(Val).count();
	}

	inline constexpr int64 GetMilliseconds() const noexcept
	{
		auto Seconds = std::chrono::floor<std::chrono::seconds>(Val);
		return std::chrono::floor<std::chrono::milliseconds>(Val - Seconds).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalMilliseconds() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::milliseconds::period>>(Val).count();
	}

	inline constexpr int64 GetMicroseconds() const noexcept
	{
		auto Milliseconds = std::chrono::floor<std::chrono::milliseconds>(Val);
		return std::chrono::floor<std::chrono::microseconds>(Val - Milliseconds).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalMicroseconds() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::microseconds::period>>(Val).count();
	}

	inline constexpr int64 GetNanoseconds() const noexcept
	{
		auto Microseconds = std::chrono::floor<std::chrono::microseconds>(Val);
		return std::chrono::floor<std::chrono::nanoseconds>(Val - Microseconds).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalNanoseconds() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::nanoseconds::period>>(Val).count();
	}

	inline constexpr int64 GetTicks() const noexcept
	{
		return Val.count();
	}

	inline constexpr std::chrono::microseconds GetSpan() const noexcept
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(Val);
	}

public:
	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromYears(T Years) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::years::period>(Years));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromMonths(T Months) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::months::period>(Months));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromDays(T Days) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::days::period>(Days));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromHours(T Hours) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::hours::period>(Hours));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromMinutes(T Minutes) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::minutes::period>(Minutes));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromSeconds(T Seconds) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::seconds::period>(Seconds));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromMilliseconds(T Milliseconds) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::milliseconds::period>(Milliseconds));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromMicroseconds(T Microseconds) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::microseconds::period>(Microseconds));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromNanoseconds(T Nanoseconds) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::nanoseconds::period>(Nanoseconds));
	}

public:
	inline constexpr operator const std::chrono::nanoseconds& () const noexcept
	{
		return Val;
	}

	template<class TRep, class TPeriod>
	inline constexpr operator std::chrono::duration<TRep, TPeriod>() const noexcept
	{
		return std::chrono::duration_cast<std::chrono::duration<TRep, TPeriod>>(Val);
	}

	template<class T>
	inline constexpr explicit operator T() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return this->template GetTotalSeconds<T>();
	}

	inline constexpr TimeSpan& operator =(const TimeSpan& Rhs) noexcept
	{
		Val = Rhs.Val;
		return *this;
	}

	inline constexpr TimeSpan& operator +=(const TimeSpan& Rhs) noexcept
	{
		Val += Rhs.Val;
		return *this;
	}

	inline constexpr TimeSpan& operator -=(const TimeSpan& Rhs) noexcept
	{
		Val -= Rhs.Val;
		return *this;
	}

	template<class T>
	inline constexpr TimeSpan& operator *=(T Rhs) noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		Val *= Rhs;
		return *this;
	}

	inline constexpr bool operator ==(const TimeSpan& Rhs) const noexcept
	{
		return Val == Rhs.Val;
	}

	inline constexpr auto operator <=>(const TimeSpan& Rhs) const noexcept
	{
		return Val <=> Rhs.Val;
	}

	inline constexpr TimeSpan operator +(const TimeSpan& Rhs) const noexcept
	{
		return Val + Rhs.Val;
	}

	inline constexpr TimeSpan operator -(const TimeSpan& Rhs) const noexcept
	{
		return Val - Rhs.Val;
	}

	template<class T>
	inline constexpr TimeSpan operator *(T Rhs) const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return Val * Rhs;
	}

public:
	static inline constexpr TimeSpan GetZero() noexcept
	{
		return TimeSpan(0ns);
	}
};