// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exceptions/InvalidOperationException.h"
#include "CoreAssert.h"
#include "DateTimeFormat.h"
#include <chrono>
#include <optional>
#include <version>

namespace libty::inline Core
{
	class DateTime
	{
	public:
		using clock = std::chrono::system_clock;
		using time_point = clock::time_point;

	private:
		std::optional<time_point> _tp;
	#if __cpp_lib_chrono >= 201907L
		const std::chrono::time_zone* _zone = std::chrono::current_zone();
	#endif

	public:
		DateTime()
		{
		}

		DateTime(const time_point& tp) : _tp(tp)
		{
		}

		constexpr bool IsValid() const
		{
			return _tp.has_value();
		}

		template<class TDateFormatter = libty::DateTimeFormat::Json>
		std::wstring ToString() const
		{
			checkf(IsValid(), TEXT("Time is not setted."));
	#if PLATFORM_WINDOWS
			return TDateFormatter::ToString(std::chrono::zoned_time(_zone, *_tp));
	#else
			return TDateFormatter::ToString(*_tp);
	#endif
		}

		auto GetTimePoint() const
		{
			checkf(IsValid(), TEXT("Time is not setted."));
			return *_tp;
		}

		static DateTime Now()
		{
			return DateTime(clock::now());
		}

		template<class TDateFormatter = libty::DateTimeFormat::Json>
		static bool TryParse(std::wstring_view time, DateTime& outDateTime)
		{
			time_point tp;
			if (!TDateFormatter::TryParse(time, tp))
			{
				outDateTime = DateTime();
				return false;
			}

			outDateTime = DateTime(tp);
			return true;
		}

		template<class Dur, class Ret = Dur>
		Ret Floor() const requires requires
		{
			{ std::chrono::floor<Dur>(std::declval<decltype(_tp->time_since_epoch())>()) };
		}
		{
			Ret ret = Ret(std::chrono::floor<Dur>(_tp->time_since_epoch()));
			return ret;
		}

		auto GetYears() const { return Floor<std::chrono::years>(); }
		auto GetMonths() const { return Floor<std::chrono::months>(); }
		auto GetDays() const { return Floor<std::chrono::days>(); }
		auto GetHours() const { return Floor<std::chrono::hours>(); }
		auto GetMinutes() const { return Floor<std::chrono::minutes>(); }
		auto GetSeconds() const { return Floor<std::chrono::seconds>(); }
		auto GetMilliseconds() const { return Floor<std::chrono::milliseconds>(); }
		auto GetMicroseconds() const { return Floor<std::chrono::microseconds>(); }
		auto GetNanoseconds() const { return Floor<std::chrono::nanoseconds>(); }

		auto GetYMD() const
		{
			auto ymd = std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(*_tp));
			return ymd;
		}

		auto GetHMD() const
		{
			auto days = std::chrono::floor<std::chrono::days>(*_tp);
			auto hms = std::chrono::hh_mm_ss(*_tp - days);
			return hms;
		}
	};
}