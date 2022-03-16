// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <chrono>
#include <sstream>
#include <string_view>
#include <version>

// @see https://en.cppreference.com/w/cpp/chrono/parse to check format strings.
namespace libty::DateTimeFormat
{
	class Json
	{
	public:
		template<class time_point>
		static std::wstring ToString(time_point tp)
		{
			return String::Format(L"{:%Y-%m-%dT%H:%M:%S%z}", tp);
		}

		template<class time_point>
		static bool TryParse(std::wstring_view text, time_point& tp)
		{
			std::wistringstream wiss(text);
#if __cpp_lib_chrono && !PLATFORM_LINUX
			return std::chrono::from_stream(wiss, L"%Y-%m-%dT%H:%M:%S%z", tp);
#else
			wchar_t ignore = 0;
			std::tm tm = {};
			int region = 0;
			wiss >> tm.tm_year >> ignore >> tm.tm_mon >> ignore >> tm.tm_mday >> ignore >> tm.tm_hour >> ignore >> tm.tm_min >> ignore >> tm.tm_sec >> ignore >> region;

#if PLATFORM_LINUX
			time_t t = timegm(&tm);
			tp = std::chrono::system_clock::from_time_t(t);
#endif
			return true;
#endif
		}
	};

	class File
	{
	public:
		template<class time_point>
		static std::wstring ToString(time_point tp)
		{
			return String::Format(L"{:%Y-%m-%d-%H-%M-%S}", tp);
		}

		template<class time_point>
		static bool TryParse(std::wstring_view text, time_point& tp)
		{
			std::wistringstream wiss(text);
#if __cpp_lib_chrono && !PLATFORM_LINUX
			return std::chrono::from_stream(wiss, L"%Y-%m-%d-%H-%M-%S", tp);
#else
			wchar_t ignore = 0;
			std::tm tm = {};
			int region = 0;
			wiss >> tm.tm_year >> ignore >> tm.tm_mon >> ignore >> tm.tm_mday >> ignore >> tm.tm_hour >> ignore >> tm.tm_min >> ignore >> tm.tm_sec;

#if PLATFORM_LINUX
			time_t t = timegm(&tm);
			tp = std::chrono::system_clock::from_time_t(t);
#endif
			return true;
#endif
		}
	};
}