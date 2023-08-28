// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "System/IntegralTypes.h"
#include <chrono>

using namespace std::chrono_literals;

class TimeSpan
{
	std::chrono::nanoseconds _val;

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
		: _val(0ns)
	{
	}

	inline constexpr TimeSpan(const std::chrono::nanoseconds& val) noexcept
		: _val(val)
	{
	}

	template<class TRep, class TPeriod>
	inline constexpr TimeSpan(const std::chrono::duration<TRep, TPeriod>& val) noexcept
		: _val(std::chrono::duration_cast<std::chrono::nanoseconds>(val))
	{
	}

public:
	inline constexpr int64 GetYears() const noexcept
	{
		return std::chrono::floor<std::chrono::years>(_val).count();
	}

	inline constexpr int64 GetMonths() const noexcept
	{
		auto years = std::chrono::floor<std::chrono::years>(_val);
		return std::chrono::floor<std::chrono::months>(_val - years).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalMonths() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::months::period>>(_val).count();
	}

	inline constexpr int64 GetDays() const noexcept
	{
		auto months = std::chrono::floor<std::chrono::months>(_val);
		return std::chrono::floor<std::chrono::days>(_val - months).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalDays() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::days::period>>(_val).count();
	}

	inline constexpr int64 GetHours() const noexcept
	{
		auto days = std::chrono::floor<std::chrono::days>(_val);
		return std::chrono::floor<std::chrono::hours>(_val - days).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalHours() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::hours::period>>(_val).count();
	}

	inline constexpr int64 GetMinutes() const noexcept
	{
		auto hours = std::chrono::floor<std::chrono::hours>(_val);
		return std::chrono::floor<std::chrono::minutes>(_val - hours).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalMinutes() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::minutes::period>>(_val).count();
	}

	inline constexpr int64 GetSeconds() const noexcept
	{
		auto minutes = std::chrono::floor<std::chrono::minutes>(_val);
		return std::chrono::floor<std::chrono::seconds>(_val - minutes).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalSeconds() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::seconds::period>>(_val).count();
	}

	inline constexpr int64 GetMilliseconds() const noexcept
	{
		auto seconds = std::chrono::floor<std::chrono::seconds>(_val);
		return std::chrono::floor<std::chrono::milliseconds>(_val - seconds).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalMilliseconds() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::milliseconds::period>>(_val).count();
	}

	inline constexpr int64 GetMicroseconds() const noexcept
	{
		auto milliseconds = std::chrono::floor<std::chrono::milliseconds>(_val);
		return std::chrono::floor<std::chrono::microseconds>(_val - milliseconds).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalMicroseconds() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::microseconds::period>>(_val).count();
	}

	inline constexpr int64 GetNanoseconds() const noexcept
	{
		auto microseconds = std::chrono::floor<std::chrono::microseconds>(_val);
		return std::chrono::floor<std::chrono::nanoseconds>(_val - microseconds).count();
	}

	template<class T = int64>
	inline constexpr T GetTotalNanoseconds() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return std::chrono::duration_cast<std::chrono::duration<T, std::chrono::nanoseconds::period>>(_val).count();
	}

	inline constexpr int64 GetTicks() const noexcept
	{
		return _val.count();
	}

	inline constexpr std::chrono::microseconds GetSpan() const noexcept
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(_val);
	}

public:
	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromYears(T years) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::years::period>(years));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromMonths(T months) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::months::period>(months));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromDays(T days) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::days::period>(days));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromHours(T hours) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::hours::period>(hours));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromMinutes(T minutes) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::minutes::period>(minutes));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromSeconds(T seconds) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::seconds::period>(seconds));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromMilliseconds(T milliseconds) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::milliseconds::period>(milliseconds));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromMicroseconds(T microseconds) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::microseconds::period>(microseconds));
	}

	template<class T> requires
		std::integral<T> ||
		std::floating_point<T>
	static inline constexpr TimeSpan FromNanoseconds(T nanoseconds) noexcept
	{
		return TimeSpan(std::chrono::duration<T, std::chrono::nanoseconds::period>(nanoseconds));
	}

public:
	inline constexpr operator const std::chrono::nanoseconds& () const noexcept
	{
		return _val;
	}

	template<class TRep, class TPeriod>
	inline constexpr operator std::chrono::duration<TRep, TPeriod>() const noexcept
	{
		return std::chrono::duration_cast<std::chrono::duration<TRep, TPeriod>>(_val);
	}

	template<class T>
	inline constexpr explicit operator T() const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return this->template GetTotalSeconds<T>();
	}

	inline constexpr TimeSpan& operator =(const TimeSpan& rhs) noexcept
	{
		_val = rhs._val;
		return *this;
	}

	inline constexpr TimeSpan& operator +=(const TimeSpan& rhs) noexcept
	{
		_val += rhs._val;
		return *this;
	}

	inline constexpr TimeSpan& operator -=(const TimeSpan& rhs) noexcept
	{
		_val -= rhs._val;
		return *this;
	}

	template<class T>
	inline constexpr TimeSpan& operator *=(T rhs) noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		_val *= rhs;
		return *this;
	}

	inline constexpr bool operator ==(const TimeSpan& rhs) const noexcept
	{
		return _val == rhs._val;
	}

	inline constexpr auto operator <=>(const TimeSpan& rhs) const noexcept
	{
		return _val <=> rhs._val;
	}

	inline constexpr TimeSpan operator +(const TimeSpan& rhs) const noexcept
	{
		return _val + rhs._val;
	}

	inline constexpr TimeSpan operator -(const TimeSpan& rhs) const noexcept
	{
		return _val + rhs._val;
	}

	template<class T>
	inline constexpr TimeSpan operator *(T rhs) const noexcept requires
		std::integral<T> ||
		std::floating_point<T>
	{
		return _val * rhs;
	}

public:
	static inline constexpr TimeSpan GetZero() noexcept
	{
		return TimeSpan(0ns);
	}
};