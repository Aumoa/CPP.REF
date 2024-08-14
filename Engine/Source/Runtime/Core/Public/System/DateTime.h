// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "System/String.h"
#include "System/TimeSpan.h"
#include "System/DateTimeFormat.Localization.h"

class DateTime
{
public:
	using clock = std::chrono::system_clock;
	using time_point = clock::time_point;
	using local_time = std::chrono::local_time<clock::duration>;

private:
	std::variant<time_point, local_time> TimePoint;

public:
	inline DateTime()
	{
	}

	inline constexpr DateTime(const time_point& InTimePoint)
		: TimePoint(InTimePoint)
	{
	}

	inline constexpr DateTime(const local_time& InLocalTime)
		: TimePoint(InLocalTime)
	{
	}

	inline constexpr DateTime(const DateTime& Rhs)
		: TimePoint(Rhs.TimePoint)
	{
	}

	template<class TDateFormatter = DateTimeFormat::Localization, class... TArgs>
	inline String ToString(TArgs&&... Args) const
	{
		return TDateFormatter::ToString(*this, std::forward<TArgs>(Args)...);
	}

	static bool TryParse(String InText, String InFormat, DateTime& OutDateTime)
	{
		std::wistringstream Ws((std::wstring)InText);
		time_point TimePoint;
		const char_t* Fmt;
		if (InFormat.IsNullTerminate())
		{
			Fmt = InFormat.c_str();
		}
		else
		{
			static thread_local std::wstring LocalCache;
			LocalCache = InFormat;
			Fmt = LocalCache.c_str();
		}

#if !defined(__GNUC__)
		if (std::chrono::from_stream(Ws, Fmt, TimePoint).rdstate() == std::ios::failbit)
		{
			OutDateTime = DateTime();
			return false;
		}
		else
#endif		
		{
			OutDateTime = DateTime(TimePoint);
			return true;
		}
	}

	template<class T>
	constexpr DateTime GetFloor() const noexcept
	{
		if (IsUTC())
		{
			return DateTime(std::chrono::floor<T>(GetTimePoint()));
		}
		else
		{
			return DateTime(std::chrono::floor<T>(GetLocalTime()));
		}
	}

public:
	inline constexpr const time_point& GetTimePoint() const
	{
		return std::get<0>(TimePoint);
	}

	inline constexpr const local_time& GetLocalTime() const
	{
		return std::get<1>(TimePoint);
	}

public:
	static DateTime Now() noexcept
	{
		return DateTime(std::chrono::current_zone()->to_local(clock::now()));
	}

	static DateTime UtcNow() noexcept
	{
		return DateTime(clock::now());
	}

	DateTime ToLocal(bool bReinterpret = false) const noexcept
	{
		if (IsUTC())
		{
			return DateTime(std::chrono::current_zone()->to_local(GetTimePoint()));
		}
		else if (bReinterpret)
		{
			auto Reinterpret = time_point(GetLocalTime().time_since_epoch());
			return DateTime(Reinterpret);
		}
		else
		{
			return *this;
		}
	}

	DateTime ToUTC(bool bReinterpret = false) const noexcept
	{
		if (IsUTC())
		{
			return *this;
		}
		else if (bReinterpret)
		{
			auto Reinterpret = local_time(GetTimePoint().time_since_epoch());
			return DateTime(Reinterpret);
		}
		else
		{
			return DateTime(std::chrono::current_zone()->to_sys(GetLocalTime()));
		}
	}

	inline constexpr bool IsValid() const noexcept { return TimePoint.index() != std::variant_npos; }
	inline constexpr bool IsUTC() const noexcept { return TimePoint.index() == 0; }

private:
	template<size_t N, class T>
	inline constexpr auto GetFloorDur() const noexcept
	{
		return std::chrono::floor<T>(std::get<N>(TimePoint)).time_since_epoch();
	}

public:
	inline constexpr auto GetYears() const noexcept
	{
		if (IsUTC())
		{
			return GetFloorDur<0, std::chrono::years>();
		}
		else
		{
			return GetFloorDur<1, std::chrono::years>();
		}
	}

	inline constexpr auto GetMonths() const noexcept
	{
		if (IsUTC())
		{
			return GetFloorDur<0, std::chrono::months>();
		}
		else
		{
			return GetFloorDur<1, std::chrono::months>();
		}
	}

	inline constexpr auto GetDays() const noexcept
	{
		if (IsUTC())
		{
			return GetFloorDur<0, std::chrono::days>();
		}
		else
		{
			return GetFloorDur<1, std::chrono::days>();
		}
	}

	inline constexpr auto GetWeekday() const noexcept
	{
		if (IsUTC())
		{
			return std::chrono::weekday(std::chrono::floor<std::chrono::days>(GetTimePoint()));
		}
		else
		{
			return std::chrono::weekday(std::chrono::floor<std::chrono::days>(GetLocalTime()));
		}
	}

	inline constexpr auto GetHours() const noexcept
	{
		if (IsUTC())
		{
			return GetFloorDur<0, std::chrono::hours>();
		}
		else
		{
			return GetFloorDur<1, std::chrono::hours>();
		}
	}

	inline constexpr auto GetMinutes() const noexcept
	{
		if (IsUTC())
		{
			return GetFloorDur<0, std::chrono::minutes>();
		}
		else
		{
			return GetFloorDur<1, std::chrono::minutes>();
		}
	}

	inline constexpr auto GetSeconds() const noexcept
	{
		if (IsUTC())
		{
			return GetFloorDur<0, std::chrono::seconds>();
		}
		else
		{
			return GetFloorDur<1, std::chrono::seconds>();
		}
	}

	inline constexpr auto GetMilliseconds() const noexcept
	{
		if (IsUTC())
		{
			return GetFloorDur<0, std::chrono::milliseconds>();
		}
		else
		{
			return GetFloorDur<1, std::chrono::milliseconds>();
		}
	}

	inline constexpr auto GetMicroseconds() const noexcept
	{
		if (IsUTC())
		{
			return GetFloorDur<0, std::chrono::microseconds>();
		}
		else
		{
			return GetFloorDur<1, std::chrono::microseconds>();
		}
	}

	inline constexpr auto GetNanoseconds() const noexcept
	{
		if (IsUTC())
		{
			return GetFloorDur<0, std::chrono::nanoseconds>();
		}
		else
		{
			return GetFloorDur<1, std::chrono::nanoseconds>();
		}
	}

	inline constexpr auto GetYMD() const noexcept
	{
		if (IsUTC())
		{
			return std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(GetTimePoint()));
		}
		else
		{
			return std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(GetLocalTime()));
		}
	}

	inline constexpr auto GetHMS() const noexcept
	{
		if (IsUTC())
		{
			return std::chrono::hh_mm_ss(GetTimePoint() - std::chrono::floor<std::chrono::days>(GetTimePoint()));
		}
		else
		{
			return std::chrono::hh_mm_ss(GetLocalTime() - std::chrono::floor<std::chrono::days>(GetLocalTime()));
		}
	}

	inline constexpr auto GetYMDHMS() const noexcept
	{
		if (IsUTC())
		{
			auto Days = std::chrono::floor<std::chrono::days>(GetTimePoint());
			return std::make_pair(std::chrono::year_month_day(Days), std::chrono::hh_mm_ss(GetTimePoint() - Days));
		}
		else
		{
			auto Days = std::chrono::floor<std::chrono::days>(GetLocalTime());
			return std::make_pair(std::chrono::year_month_day(Days), std::chrono::hh_mm_ss(GetLocalTime() - Days));
		}
	}

	inline constexpr TimeSpan operator -(const DateTime& Rhs) const noexcept
	{
		if (IsUTC() == Rhs.IsUTC())
		{
			if (IsUTC())
			{
				return TimeSpan(GetTimePoint() - Rhs.GetTimePoint());
			}
			else
			{
				return TimeSpan(GetLocalTime() - Rhs.GetLocalTime());
			}
		}
		else
		{
			if (IsUTC())
			{
				return TimeSpan(GetTimePoint() - Rhs.ToUTC().GetTimePoint());
			}
			else
			{
				return TimeSpan(ToUTC().GetTimePoint() - Rhs.GetTimePoint());
			}
		}
	}

	inline constexpr DateTime operator +(const TimeSpan& Rhs) const noexcept
	{
		if (IsUTC())
		{
			return DateTime(GetTimePoint() + Rhs.GetSpan());
		}
		else
		{
			return DateTime(GetLocalTime() + Rhs.GetSpan());
		}
	}

	inline constexpr DateTime operator -(const TimeSpan& Rhs) const noexcept
	{
		if (IsUTC())
		{
			return DateTime(GetTimePoint() - Rhs.GetSpan());
		}
		else
		{
			return DateTime(GetLocalTime() - Rhs.GetSpan());
		}
	}
};